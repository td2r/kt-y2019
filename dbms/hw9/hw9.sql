CREATE EXTENSION pgcrypto;

---- CREATE TABLES --------
DROP TABLE IF EXISTS Flights CASCADE;

CREATE TABLE IF NOT EXISTS Flights (
    FlightId INTEGER PRIMARY KEY NOT NULL,
    FlightTime TIMESTAMP NOT NULL,
    PlaneId INTEGER NOT NULL
);

DROP TABLE IF EXISTS Seats CASCADE;

CREATE TABLE IF NOT EXISTS Seats(
    PlaneId INTEGER NOT NULL,
    SeatNo VARCHAR(4) NOT NULL,
    PRIMARY KEY (PlaneId, SeatNo)
);

DROP TABLE IF EXISTS Users CASCADE;

CREATE TABLE IF NOT EXISTS Users(
    UserId INTEGER PRIMARY KEY NOT NULL,
    Pass VARCHAR(64) NOT NULL
);

DROP TABLE IF EXISTS Booking CASCADE;

CREATE TABLE IF NOT EXISTS Booking(
    FlightId INTEGER NOT NULL,
    SeatNo VARCHAR(4) NOT NULL,
    UserId INTEGER,
    IsBought BOOLEAN NOT NULL,
    ReservedUntil TIMESTAMP,

    PRIMARY KEY (FLightId, SeatNo),
    FOREIGN KEY (FlightId) REFERENCES Flights(FlightId)
);

--------------- TRIGGERS AND CONSTRAINTS ----------------

ALTER TABLE Booking
ADD CONSTRAINT ReservedTimeExists
CHECK (IsBought OR ReservedUntil IS NOT NULL);

CREATE OR REPLACE FUNCTION HashPass()
RETURNS trigger AS
$$
BEGIN
    NEW.Pass = CRYPT(NEW.Pass, GEN_SALT('md5'));
    RETURN NEW;
END;
$$
LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER HashPass
    BEFORE INSERT
    ON Users
    FOR EACH ROW
EXECUTE PROCEDURE HashPass();

-------- TEST DATA ---------------------

DELETE FROM Booking;
DELETE FROM Users;
DELETE FROM Seats;
DELETE FROM Flights;

INSERT INTO Flights VALUES (1, CAST('November 20, 2022 10:00:00' AS TIMESTAMP), 1);
INSERT INTO Seats VALUES (1, '001A'),
                         (1, '001B'),
                         (1, '001C'),
                         (1, '001D');
INSERT INTO Booking VALUES (1, '001D', 11, FALSE, NOW() + INTERVAL '3 day');

INSERT INTO Users (UserId, Pass) VALUES (11, 'abc');

------- HELP FUNCTIONS -----------------

CREATE OR REPLACE FUNCTION ValidateUser(UserId INTEGER, Pass VARCHAR(256))
RETURNS BOOLEAN
LANGUAGE plpgsql
AS
$$
DECLARE
    PassHash VARCHAR(64);
BEGIN
    PassHash = (
        SELECT Users.Pass
        FROM Users
        WHERE Users.UserId = ValidateUser.UserId
    );
    RETURN CRYPT(Pass, PassHash) = PassHash;
END;
$$;

CREATE OR REPLACE FUNCTION ValidateFlightAndSeat(FlightId INTEGER, SeatNo VARCHAR(4))
RETURNS BOOLEAN
LANGUAGE plpgsql
AS
$$
DECLARE
    FlightId_ ALIAS FOR $1;
    SeatNo_ ALIAS FOR $2;
BEGIN
    RETURN EXISTS(
        SELECT *
        FROM Seats
        WHERE PlaneId = (
                SELECT PlaneId
                FROM Flights
                WHERE Flights.FlightId = FlightId_ AND FlightTime > NOW()
            ) AND Seats.SeatNo = SeatNo_
    );
END;
$$;

-- Not bought and not reserved
CREATE OR REPLACE FUNCTION ValidateSeatFree(FlightId INTEGER, SeatNo VARCHAR(4))
    RETURNS BOOLEAN
    LANGUAGE plpgsql
AS
$$
DECLARE
    FlightId_ ALIAS FOR $1;
    SeatNo_ ALIAS FOR $2;
BEGIN
    RETURN NOT EXISTS(
        SELECT *
        FROM Booking
        WHERE Booking.FlightId = FlightId_ AND
              Booking.SeatNo = SeatNo_ AND
              (IsBought OR ReservedUntil > NOW())
    );
END;
$$;

CREATE OR REPLACE FUNCTION ValidateSeatExistsAndFree(FlightId INTEGER, SeatNo VARCHAR(4))
RETURNS BOOLEAN
LANGUAGE plpgsql
AS
$$
DECLARE
BEGIN
    RETURN ValidateFlightAndSeat(FlightId, SeatNo) AND
           ValidateSeatFree(FlightId, SeatNo);
END;
$$;

CREATE OR REPLACE FUNCTION ValidateReservedBy(FlightId INTEGER,
                                              SeatNo VARCHAR(4),
                                              UserId INTEGER)
RETURNS BOOLEAN
LANGUAGE plpgsql
AS
$$
DECLARE
    FlightId_ ALIAS FOR $1;
    SeatNo_ ALIAS FOR $2;
    UserId_ ALIAS FOR $3;
BEGIN
    RETURN EXISTS(
        SELECT *
        FROM Booking
        WHERE Booking.FlightId = FlightId_ AND
              Booking.SeatNo = SeatNo_ AND
              Booking.UserId = UserId_ AND
              NOT IsBought AND
              NOW() < ReservedUntil
    );
END;
$$;

CREATE OR REPLACE PROCEDURE DeleteExpiredReservation(FlightId INTEGER, SeatNo VARCHAR(4))
LANGUAGE plpgsql
AS
$$
DECLARE
    FlightId_ ALIAS FOR $1;
    SeatNo_ ALIAS FOR $2;
BEGIN
    DELETE FROM Booking
    WHERE Booking.FlightId = FlightId_ AND
          Booking.SeatNo = SeatNo_ AND
          NOT IsBought AND
          ReservedUntil < NOW();
END;
$$;

CREATE OR REPLACE PROCEDURE DeleteAllExpiredReservation()
LANGUAGE plpgsql
AS
$$
BEGIN
    DELETE FROM Booking
    WHERE NOT IsBought AND ReservedUntil < NOW();
END;
$$;

CREATE OR REPLACE VIEW FlightStats AS
SELECT UserId,
       FlightId,
       COUNT(*) - COUNT(IsBought) > 0 AS CanReserve,
       COUNT(*) - COUNT(IsBought) +
           COUNT(CASE WHEN NOT IsBought AND Booking.UserId = Users.UserId THEN 1 END) > 0 AS CanBuy,
       (COUNT(*) - COUNT(IsBought))::INTEGER AS Free,
       COUNT(CASE WHEN NOT IsBought THEN 1 END)::INTEGER AS Reserved,
       COUNT(CASE WHEN IsBought THEN 1 END)::INTEGER AS Bought
FROM Flights CROSS JOIN Users NATURAL JOIN Seats NATURAL LEFT JOIN Booking
GROUP BY UserId, FlightId;

------- TASK FUNCTIONS -----------------
-------- 1-----------------------------

CREATE OR REPLACE FUNCTION FreeSeats(FlightId INTEGER)
RETURNS TABLE(SeatNo VARCHAR(4)) LANGUAGE plpgsql
AS
$$
DECLARE
    CurrentTime TIMESTAMP;
    FlightId_ ALIAS FOR $1;
BEGIN
    CurrentTime = NOW()::TIMESTAMP;
    IF CurrentTime > (SELECT FlightTime FROM Flights WHERE Flights.FlightId = FlightId_) THEN
        RETURN;
    END IF;
    RETURN QUERY
        SELECT Seats.SeatNo
        FROM Seats
        WHERE PlaneId = (SELECT PlaneId FROM Flights WHERE Flights.FlightId = FlightId_)
        EXCEPT
        SELECT Booking.SeatNo
        FROM Booking
        WHERE Booking.FlightId = FlightId_ AND
              (IsBought OR ReservedUntil > CurrentTime);
END;
$$;

SELECT * FROM FreeSeats(1);

---------------- 2------------------------

CREATE OR REPLACE FUNCTION Reserve(UserId INTEGER, Pass VARCHAR(256), FlightId INTEGER, SeatNo VARCHAR(4))
RETURNS BOOLEAN
LANGUAGE plpgsql
AS
$$
BEGIN
    IF NOT (ValidateUser(UserId, Pass) AND ValidateSeatExistsAndFree(FlightId, SeatNo))
    THEN
        RETURN FALSE;
    END IF;
    CALL DeleteExpiredReservation(FlightId, SeatNo);
    INSERT INTO Booking (FlightId, SeatNo, UserId, IsBought, ReservedUntil)
        VALUES (FlightId, SeatNo, UserId, FALSE, NOW() + INTERVAL '3 day');
    RETURN TRUE;
    EXCEPTION
        WHEN sqlstate '23505' THEN -- Someone reserved before us
        RETURN FALSE;
END;
$$;

------------------- 3 --------------------------------

CREATE OR REPLACE FUNCTION ExtendReservation(UserId INTEGER,
                                             Pass VARCHAR(256),
                                             FlightId INTEGER,
                                             SeatNo VARCHAR(4))
RETURNS BOOLEAN
    LANGUAGE plpgsql
AS
$$
DECLARE
    UserId_ ALIAS FOR $1;
    Pass_ ALIAS FOR $2;
    FlightId_ ALIAS FOR $3;
    SeatNo_ ALIAS FOR $4;
BEGIN
    IF NOT (ValidateUser(UserId_, Pass_))
    THEN
        RETURN FALSE;
    END IF;
    UPDATE Booking
    SET ReservedUntil = NOW() + INTERVAL '3 day'
    WHERE Booking.FlightId = FlightId_ AND
            Booking.SeatNo = SeatNo_ AND
          NOT IsBought AND
          ReservedUntil > NOW();
    RETURN FOUND;
END;
$$;

DELETE FROM Booking WHERE (FlightId = 1 AND SeatNo = '001B');
INSERT INTO Booking (FlightId, SeatNo, UserId, IsBought, ReservedUntil)
VALUES (1, '001B', 11, FALSE, CAST('November 15, 2022 10:00:00' AS TIMESTAMP));

SELECT ExtendReservation(11, 'abc', 1, '001B');

----------- 4 --------------------

CREATE OR REPLACE FUNCTION BuyFree(FlightId INTEGER, SeatNo VARCHAR(4))
RETURNS BOOLEAN
LANGUAGE plpgsql
AS
$$
DECLARE
    FlightId_ ALIAS FOR $1;
    SeatNo_ ALIAS FOR $2;
BEGIN
    IF NOT ValidateSeatExistsAndFree(FlightId_, SeatNo_) THEN
        RETURN FALSE;
    END IF;
    CALL DeleteExpiredReservation(FlightId_, SeatNo_);
    INSERT INTO Booking (FlightId, SeatNo, UserId, IsBought, ReservedUntil)
        VALUES (FlightId_, SeatNo_, NULL, TRUE, NULL);
    RETURN TRUE;
EXCEPTION
    WHEN sqlstate '23505' THEN -- Someone bought before us
    RETURN FALSE;
END;
$$;

------------- 5 ------------------------

CREATE OR REPLACE FUNCTION BuyReserved(UserId INTEGER, Pass VARCHAR(256), FlightId INTEGER, SeatNo VARCHAR(4))
    RETURNS BOOLEAN
    LANGUAGE plpgsql
AS
$$
DECLARE
    UserId_ ALIAS FOR $1;
    Pass_ ALIAS FOR $2;
    FlightId_ ALIAS FOR $3;
    SeatNo_ ALIAS FOR $4;
BEGIN
    IF NOT ValidateUser(UserId_, Pass_) THEN
        RETURN FALSE;
    END IF;
    UPDATE Booking
    SET IsBought = TRUE,
        ReservedUntil = NULL
    WHERE Booking.FlightId = FlightId_ AND
          Booking.SeatNo = SeatNo_ AND
          NOT IsBought AND
          ReservedUntil > NOW();
    RETURN FOUND;
END;
$$;

----------------------- 6 -----------------------

CREATE OR REPLACE FUNCTION FlightsStatistics(UserId INTEGER, Pass VARCHAR(256))
RETURNS TABLE(
    FlightId INTEGER,
    CanReserve BOOLEAN,
    CanBuy BOOLEAN,
    Free INTEGER,
    Reserved INTEGER,
    Bought INTEGER
 )
LANGUAGE plpgsql
AS
$$
DECLARE
    UserId_ ALIAS FOR $1;
BEGIN
    IF NOT ValidateUser(UserId_, Pass) THEN
        RETURN;
    END IF;
    CALL DeleteAllExpiredReservation();
    RETURN QUERY (
        SELECT fs.FLightId, fs.CanReserve, fs.CanBuy, fs.Free, fs.Reserved, fs.Bought
        FROM FlightStats fs
        WHERE fs.UserId = UserId_
    );
END;
$$;

--------------------- 7 -------------------------------------

CREATE OR REPLACE FUNCTION FlightStat(UserId INTEGER, Pass VARCHAR(256), FlightId INTEGER)
RETURNS TABLE(
    CanReserve BOOLEAN,
    CanBuy BOOLEAN,
    Free INTEGER,
    Reserved INTEGER,
    Bought INTEGER
)
LANGUAGE plpgsql
AS
$$
DECLARE
    UserId_ ALIAS FOR $1;
    FlightId_ ALIAS FOR $3;
BEGIN
    IF NOT ValidateUser(UserId_, Pass) THEN
        RETURN;
    END IF;
    CALL DeleteAllExpiredReservation();
    RETURN QUERY (
        SELECT fs.CanReserve, fs.CanBuy, fs.Free, fs.Reserved, fs.Bought
        FROM FlightStats fs
        WHERE fs.UserId = UserId_ AND fs.FlightId = FlightId_
    );
END;
$$;

---------------------- 8 ------------------------------

CREATE OR REPLACE PROCEDURE CompressSeats(FlightId INTEGER)
LANGUAGE plpgsql
AS
$$
DECLARE
    FlightId_ ALIAS FOR $1;

    SeatsCursor CURSOR FOR
        SELECT SeatNo
        FROM Flights NATURAL JOIN Seats
        WHERE Flights.FlightId = FlightId_
        ORDER BY SeatNo;

    BoughtCursor CURSOR FOR
        SELECT SeatNo
        FROM Booking
        WHERE Booking.FlightId = FlightId_ AND IsBought
        ORDER BY SeatNo;

    ReservedCursor CURSOR FOR
        SELECT SeatNo
        FROM Booking
        WHERE Booking.FlightId = FlightId_ AND NOT IsBought
        ORDER BY SeatNo;

    SeatNoVar VARCHAR(4);
BEGIN
    CALL DeleteAllExpiredReservation();
    OPEN SeatsCursor;
    FOR _ IN BoughtCursor
    LOOP
        FETCH SeatsCursor INTO SeatNoVar;
        UPDATE Booking SET SeatNo = SeatNoVar WHERE CURRENT OF BoughtCursor;
    END LOOP;
    FOR _ IN ReservedCursor
    LOOP
        FETCH SeatsCursor INTO SeatNoVar;
        UPDATE Booking SET SeatNo = SeatNoVar WHERE CURRENT OF ReservedCursor;
    END LOOP;
END;
$$;

