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

SELECT StudentId, GroupId, CourseId, Mark
FROM Students NATURAL JOIN
     Groups NATURAL JOIN
     Lecturers NATURAL JOIN
     Plan NATURAL JOIN
     Marks;

-- SECTION 6.1
CREATE FUNCTION NoExtraMarksPrecondition()
RETURNS VOID
LANGUAGE plpgsql AS
    $$DECLARE
        extraMark RECORD;
    BEGIN
        FOR extraMark IN
            SELECT StudentId, CourseId
            FROM Marks
            WHERE CourseId NOT IN (
                SELECT CourseId
                FROM Plan
                WHERE Plan.GroupId = (
                    SELECT GroupId
                    FROM Students
                    WHERE Students.StudentId = Marks.StudentId
                )
            )
        LOOP
            RAISE EXCEPTION
                'Student (%) has mark on course (%) not from his plan',
                extraMark.StudentId,
                extraMark.CourseId;
        END LOOP;
    END;$$;

CREATE FUNCTION triggerMarkModification()
RETURNS TRIGGER
LANGUAGE plpgsql
AS $$BEGIN
    IF (NEW.CourseId NOT IN (
        SELECT CourseId
        FROM Plan
        WHERE GroupId = (
            SELECT GroupId
            FROM Students
            WHERE Students.StudentId = NEW.StudentId
        )
    )) THEN
       RAISE EXCEPTION
           'Insert mark of student (id=%) for course (id=%) not in his plan',
           NEW.StudentId,
           NEW.CourseId;
    ELSE
        RETURN NEW;
    END IF;
END$$;

CREATE TRIGGER onInsertMark
    BEFORE INSERT OR UPDATE
    OF StudentId, CourseId
    ON Marks
    FOR EACH ROW
    EXECUTE FUNCTION triggerMarkModification();

-- DROP TRIGGER onInsertMark ON Marks;
-- DROP FUNCTION triggerMarkModification();
-- INSERT INTO Marks VALUES (1, 8, 5);
-- INSERT INTO Marks VALUES (8, 5, 5);
-- DELETE FROM Marks WHERE StudentId = 8 AND CourseId = 8 AND Mark = 5;

CREATE FUNCTION triggerPlanRemove()
    RETURNS TRIGGER
    LANGUAGE plpgsql
AS $$BEGIN
    IF (EXISTS (SELECT * FROM Marks WHERE CourseId = OLD.CourseId)) THEN
        RAISE EXCEPTION
            'Deleting course (id=%) which students have marks for',
            OLD.CourseId;
    ELSE
        RETURN NEW;
    END IF;
END$$;

CREATE TRIGGER onDeletingCourse
    BEFORE DELETE
    ON Plan
    FOR EACH ROW
    EXECUTE FUNCTION triggerPlanRemove();

-- DROP TRIGGER onDeletingCourse ON Plan;
-- DROP FUNCTION triggerPlanRemove;

DO $$BEGIN
    PERFORM NoExtraMarksPrecondition();
END$$;

--SECTION 6.2
CREATE FUNCTION SameMarksPrecondition()
    RETURNS VOID
    LANGUAGE plpgsql AS
$$DECLARE
    badStudent RECORD;
BEGIN
    FOR badStudent IN
        SELECT StudentId, GroupId, cnt1, cnt2
        FROM (
            SELECT Students.StudentId, GroupId, COUNT(DISTINCT CourseId) AS cnt1
            FROM Students LEFT JOIN Marks ON Students.StudentId = Marks.StudentId
            GROUP BY Students.StudentId
        ) studentsCounts NATURAL JOIN (
            SELECT GroupId, COUNT(DISTINCT CourseId) AS cnt2
            FROM Students LEFT JOIN Marks ON Students.StudentId = Marks.StudentId
            GROUP BY GroupId
        ) groupsCounts
        WHERE cnt1 != cnt2
    LOOP
        RAISE EXCEPTION
            'Student (id=%) has mark for % different courses, while his group (id=%) for %',
            badStudent.StudentId,
            badStudent.cnt1,
            badStudent.GroupId,
            badStudent.cnt2;
    END LOOP;
END;$$;

-- DROP FUNCTION SameMarksPrecondition();

CREATE FUNCTION CheckSameMarksModifyMarks()
    RETURNS TRIGGER
    LANGUAGE plpgsql
AS $$BEGIN
    PERFORM SameMarksPrecondition();
    RETURN NEW;
END$$;

CREATE TRIGGER onMarkModify
    AFTER INSERT OR UPDATE OF StudentId, CourseId OR DELETE
    ON Marks
    FOR EACH ROW
    EXECUTE FUNCTION CheckSameMarksModifyMarks();

-- DROP TRIGGER onMarkModify ON Marks;
-- DROP FUNCTION CheckSameMarksModifyMarks();

CREATE FUNCTION CheckSameMarksModifyStudents()
    RETURNS TRIGGER
    LANGUAGE plpgsql
AS $$BEGIN
    PERFORM SameMarksPrecondition();
    RETURN NEW;
END$$;

CREATE TRIGGER onStudentsModify
    AFTER INSERT OR UPDATE OF StudentId, GroupId
    ON Students
    FOR EACH ROW
EXECUTE FUNCTION CheckSameMarksModifyStudents();

-- DROP TRIGGER onStudentsModify ON Students;
-- DROP FUNCTION CheckSameMarksModifyStudents();

DO $$BEGIN
    PERFORM SameMarksPrecondition();
END $$;

INSERT INTO Students VALUES (228, 'pudge', 5);
INSERT INTO Marks VALUES (228, 5, 3);
DELETE FROM Students WHERE StudentId = 228;
DELETE FROM Marks WHERE StudentId = 228;

-- SECTION 6.3
CREATE OR REPLACE FUNCTION RollbackMark()
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
    EXECUTE FUNCTION RollbackMark();

-- DROP TRIGGER PreserveMarks ON Marks;
-- DROP FUNCTION RollbackMark();

-- UPDATE Marks
-- SET Mark = 1
-- WHERE StudentId = 8 AND CourseId = 8;

-- 5.1.1
-- 5.2.1
-- 5.3.1
CREATE UNIQUE INDEX SIdHashIndex ON Students USING HASH (StudentId);

-- 5.1.2
CREATE UNIQUE INDEX SNameSIdGIdBtreeIndex ON Students USING BTREE (StudentName, StudentId, GroupId);

-- 5.2.1
-- 5.2.2
CREATE UNIQUE INDEX GIdHashIndex ON Groups USING HASH (GroupId);

-- 5.3.1
-- 5.3.2
CREATE INDEX CIdMarkHashIndex ON Marks USING HASH (CourseId, Mark);

-- 5.3.2
CREATE UNIQUE INDEX CNameCIdBtreeIndex ON Courses USING BTREE (CourseName, CourseId);



