CREATE TABLE Groups(
    GroupId INT,
    GroupName VARCHAR(10),
    PRIMARY KEY (GroupId),
    UNIQUE (GroupName)
);

CREATE TABLE Students(
    StudentId INT,
    StudentName VARCHAR(50),
    GroupId INT,
    PRIMARY KEY (StudentId),
    FOREIGN KEY (GroupId) REFERENCES Groups(GroupId)
);

CREATE TABLE Courses(
    CourseId INT,
    CourseName VARCHAR(50),
    PRIMARY KEY (CourseId)
);

CREATE TABLE Lecturers(
    LecturerId INT,
    LecturerName VARCHAR(50),
    PRIMARY KEY (LecturerId)
);

CREATE TABLE Plan(
    GroupId INT,
    CourseId INT,
    LecturerId INT,
    PRIMARY KEY (GroupId, CourseId),
    FOREIGN KEY(GroupId) REFERENCES Groups(GroupId),
    FOREIGN KEY(CourseId) REFERENCES Courses(CourseId),
    FOREIGN KEY(LecturerId) REFERENCES Lecturers(LecturerId)
);

CREATE TABLE Marks(
    StudentId INT,
    CourseId INT,
    Mark INT,
    PRIMARY KEY (StudentId, CourseId),
    FOREIGN KEY (StudentId) REFERENCES Students(StudentId),
    FOREIGN KEY (CourseId) REFERENCES Courses(CourseId)
);

------------------------------------------------------------------------------------------------------------------------

INSERT INTO Groups
VALUES (5, 'M3235'),
       (8, 'M3438');

INSERT INTO Students
VALUES (1, 'Иванов И.И.', 5),
       (2, 'Петров П.П.', 5),
       (3, 'Сидров С.С.', 5),
       (5, 'Григорьев Г.Г.', 8),
       (8, 'Костин К.К.', 8);

INSERT INTO Courses
VALUES (1, 'Язык программирования Java'),
       (4, 'Алгоритмы и структуры данных'),
       (5, 'Физкультура'),
       (8, 'Базы данных'),
       (9, 'Параллельные алгоритмы'),
       (11, 'Проектирование ППО');

INSERT INTO Lecturers
VALUES (1, 'Корнеев Г.К.'),
       (2, 'Аксенов В.А.'),
       (4, 'Мухаммадеев Ф.Ф.'),
       (5, 'Журба Я.О.'),
       (7, 'Киракозов А.С.'),
       (10, 'Маврин П.Е.');

INSERT INTO Plan
VALUES (5, 1, 1),
       (5, 4, 10),
       (5, 5, 5),
       (8, 8, 1),
       (8, 9, 2),
       (8, 11, 7),
       (8, 5, 4);

INSERT INTO Marks
VALUES (1, 1, 5),
       (1, 4, 5),
       (1, 5, 5),
       (2, 1, 3),
       (2, 4, 4),
       (2, 5, 5),
       (3, 1, 2),
       (3, 4, 4),
       (3, 5, 3),
       (5, 8, 4),
       (5, 9, 4),
       (8, 8, 2),
       (8, 9, 3);

------------------------------------------------------------------------------------------------------------------------

SELECT *
FROM Students NATURAL JOIN
     Groups NATURAL JOIN
     Lecturers NATURAL JOIN
     Plan NATURAL JOIN
     Marks;


-- SECTION 6.3
CREATE OR REPLACE FUNCTION rollbackMark()
RETURNS TRIGGER LANGUAGE plpgsql AS
    $$BEGIN
        NEW.Mark := OLD.Mark;
        RETURN NEW;
    END;$$;

CREATE TRIGGER PreserveMarks
    BEFORE UPDATE
    ON Marks
    FOR EACH ROW
    WHEN (NEW.Mark < OLD.Mark)
    EXECUTE FUNCTION rollbackMark();

UPDATE Marks
SET Mark = 1
WHERE StudentId = 8 AND CourseId = 8;

SELECT *
FROM Students NATURAL JOIN
     Groups NATURAL JOIN
     Lecturers NATURAL JOIN
     Plan NATURAL JOIN
     Marks;

