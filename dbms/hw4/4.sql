CREATE TABLE Students(
    StudentId int,
    StudentName varchar(50),
    PRIMARY KEY (StudentId)
);

CREATE TABLE Groups(
    GroupId int,
    GroupName varchar(10),
    PRIMARY KEY (GroupId)
);

CREATE TABLE Courses(
    CourseId int,
    CourseName varchar(50),
    PRIMARY KEY (CourseId)
);

CREATE TABLE Lecturers(
    LecturerId int,
    LecturerName varchar(50),
    PRIMARY KEY (LecturerId)
);

CREATE TABLE StudentGroup(
    StudentId int,
    GroupId int,
    PRIMARY KEY (StudentId),
    FOREIGN KEY (StudentId) REFERENCES Students(StudentId),
    FOREIGN KEY (GroupId) REFERENCES Groups(GroupId)
);

CREATE TABLE Teaching(
    GroupId int,
    CourseId int,
    LecturerId int,
    PRIMARY KEY (GroupId, CourseId),
    FOREIGN KEY(GroupId) REFERENCES Groups(GroupId),
    FOREIGN KEY(CourseId) REFERENCES Courses(CourseId),
    FOREIGN KEY(LecturerId) REFERENCES Lecturers(LecturerId)
);

CREATE TABLE Scoring(
    StudentId int,
    CourseId int,
    Mark int,
    PRIMARY KEY (StudentId, CourseId),
    FOREIGN KEY (StudentId) REFERENCES Students(StudentId),
    FOREIGN KEY (CourseId) REFERENCES Courses(CourseId)
);

INSERT INTO Students (StudentId, StudentName) VALUES (1, 'Joshua Griffin');
INSERT INTO Students (StudentId, StudentName) VALUES (5, 'Tyler Jackson');
INSERT INTO Students (StudentId, StudentName) VALUES (7, 'Jeffrey Burns');
INSERT INTO Students (StudentId, StudentName) VALUES (8, 'Seth Malone');
INSERT INTO Groups (GroupId, GroupName) VALUES (3, 'A1142');
INSERT INTO Groups (GroupId, GroupName) VALUES (5, 'B2128');
INSERT INTO Courses (CourseId, CourseName) VALUES (6, 'Jurisprudence');
INSERT INTO Courses (CourseId, CourseName) VALUES (9, 'Economics');
INSERT INTO Courses (CourseId, CourseName) VALUES (10, 'Math Statistics');
INSERT INTO Lecturers (LecturerId, LecturerName) VALUES (8, 'Martin Torres');
INSERT INTO Lecturers (LecturerId, LecturerName) VALUES (12, 'Donald Ford');
INSERT INTO Lecturers (LecturerId, LecturerName) VALUES (13, 'Michael Allen');
INSERT INTO StudentGroup (StudentId, GroupId) VALUES (1, 3);
INSERT INTO StudentGroup (StudentId, GroupId) VALUES (5, 5);
INSERT INTO StudentGroup (StudentId, GroupId) VALUES (7, 5);
INSERT INTO StudentGroup (StudentId, GroupId) VALUES (8, 3);
INSERT INTO Teaching (GroupId, CourseId, LecturerId) VALUES (3, 6, 8);
INSERT INTO Teaching (GroupId, CourseId, LecturerId) VALUES (3, 9, 12);
INSERT INTO Teaching (GroupId, CourseId, LecturerId) VALUES (5, 6, 8);
INSERT INTO Teaching (GroupId, CourseId, LecturerId) VALUES (5, 10, 13);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (1, 6, 5);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (1, 9, 2);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (5, 6, 4);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (5, 10, 4);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (7, 6, 2);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (7, 10, 5);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (8, 6, 3);
INSERT INTO Scoring (StudentId, CourseId, Mark) VALUES (8, 9, 3);



DROP TABLE Scoring;
DROP TABLE Teaching;
DROP TABLE StudentGroup;
DROP TABLE Lecturers;
DROP TABLE Courses;
DROP TABLE Groups;
DROP TABLE Students;
