--Every movie has a unique identification number.
--Every movie must have a title.
--Every actor must have a date of birth.


-- BEGIN Movie TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- PRIMARY KEY violations for Movie table:
-- Movie: id is the primary key, and must be unique
INSERT INTO Movie VALUES(3895, "Batman", 1990, "PG-17", "Warner Bros.");
-- The above would fail because the id 3895 already exists.
-- ERROR 1062 (23000): Duplicate entry '3895' for key 1

-- FOREIGN KEY violations for Movie table:
DROP TABLE IF EXISTS Movie;
-- Fails if attempted before deletion of referencing tables.
-- ERROR 1217 (23000): Cannot delete or update a parent row: a foreign key constraint fails

-- CHECK violations for Movie table:
-- Movie id must not be negative.
INSERT INTO Movie 
VALUES(-3895, 'Batman', 1990, 'PG-17', 'Warner Bros');
-- Should fail, but does not due to lack of CHECK support in MySQL 
-- (no error generated).

-- NOT NULL and TYPE violations for Movie table:
INSERT INTO Movie 
VALUES(1+(SELECT MaxMovieID.id FROM MaxMovieID), NULL, 1990, 'PG-17', 'Warner Bros');
-- Should fail because title is marked NOT NULL.
-- ERROR 1048 (23000): Column 'title' cannot be null.

INSERT INTO Movie
VALUES(1+(SELECT MaxMovieID.id FROM MaxMovieID), 'Catman', Nineteen ninety, 'PG-17', 'Warner Bros');
-- Should fail because year attribute is specified as TYPE INT.
-- ERROR 1064 (42000): You have an error in your SQL syntax; check the manual that corresponds to your MySQL server version for the right syntax to use near 'ninety, 'PG-17', 'Warner Bros')' at line 2

-- BEGIN Actor TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- PRIMARY KEY violations for Actor table below.
-- Actor id is the primary key, must be unique.
INSERT INTO Actor 
VALUES(68625, "Chemelekov", "Rossen", "Male", 1984-06-19, NULL);
-- ERROR 1062 (23000): Duplicate entry '39050' for key 1

-- FOREIGN KEY violations for Actor table:
DROP TABLE IF EXISTS Actor;
-- Fails if attempted before deletion of referencing tables.
-- ERROR 1217 (23000): Cannot delete or update a parent row: a foreign key constraint fails


-- NOT NULL, TYPE, and CHECK violations for Actor table:
-- Actor id must not be negative
INSERT INTO Actor 
VALUES(-68625, "Chemelekov", "Rossen", "Male", 1984-06-19, NULL);
-- Should fail, but does not due to lack of CHECK support in MySQL 
-- (no error generated).

-- Actor must have a dob.
INSERT INTO Actor VALUES(666666, "Chemelekov", "Rossen", "Male", NULL, NULL);
-- Should fail because the attribute dob is marked NOT NULL.
-- ERROR 1048 (23000): Column 'dob' cannot be null

-- Actor must have a first name.
INSERT INTO Actor VALUES(1+(SELECT MaxPersonID.id FROM MaxPersonID),
						 "McQueen", NULL, "Male", 1930-03-24, 1980-11-07);
-- Fails because the first name is NULL.
-- ERROR 1048 (23000): Column 'first' cannot be null


-- BEGIN Director TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- FOREIGN KEY violations for Director table:
-- Director id is the primary key, must be unique
INSERT INTO Director VALUES(39050, "Chemelekov", "Rossen", 1984-06-19, NULL);
-- fails because id already exists
-- ERROR 1062 (23000): Duplicate entry '39050' for key 1

-- NOT NULL, TYPE, and CHECK violations for Director table:
-- Director id must be positive
INSERT INTO Director VALUES(-666666, "Chemelekov", "Rossen", 1984-06-19, NULL);
-- Theoretically fails because id is negative and we're using 
-- CHECK to make sure it's positive.

-- Director must have a dob
INSERT INTO Director VALUES(666666, "Chemelekov", "Rossen", NULL, NULL);
-- fails because this director was never born, that is, dob is NULL
-- ERROR 1048 (23000): Column 'dob' cannot be null


-- BEGIN MovieGenre TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- FOREIGN KEY violations:
-- Assume that the MaxMovieID.id attribute is currently set to 70000.
INSERT INTO MovieGenre VALUES(71203, 'Horror');
-- Fails because mid = 71203 is not in Movie.id (foreign key constraint).
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails
-- (`CS143/MovieGenre`, CONSTRAINT `MovieGenre_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`) ON DELETE CASCADE ON UPDATE CASCADE)

-- BEGIN MovieDirector TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- FOREIGN KEY violations:
-- Assume that the MaxMovieID.id attribute is currently set to 7000 and the 
-- MaxPersonID.id attribute is currently set to 69500.
INSERT INTO MovieDirector VALUES(7120, 69999);
-- Fails because mid = 7120 is not in Movie.id (foreign key constraint).
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails
-- (`CS143/MovieDirector`, CONSTRAINT `MovieDirector_ibfk_1` FOREIGN KEY (`mid`) REFERENCES
-- `Movie` (`id`) ON DELETE CASCADE ON UPDATE CASCADE)

-- BEGIN MovieActor TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- FOREIGN KEY violations:
-- Assume that the MaxPersonID.id attribute is currently set to 69500.
INSERT INTO MovieActor VALUES(3450, 69999, 'Dirty Harry');
-- Fails because 69999 is not in Actor.id.
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails
-- (`CS143/MovieActor`, CONSTRAINT `MovieActor_ibfk_2` FOREIGN KEY (`aid`) REFERENCES
-- `Actor` (`id`) ON DELETE CASCADE ON UPDATE CASCADE)

-- BEGIN Review TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- Review constraints: reviewer must have a name
--		   			   reviewer must leave a rating
--		               rating must be between 0 and 5
--		               foreign key referencing Movie(id).

-- FOREIGN KEY violations:
INSERT INTO Review VALUES('Make money quick', '2014-01-19 03:14:07', NULL, 1, 'SPAM!');
-- Fails because mid parameter is NULL (foreign key constraint)
-- ERROR 1048 (23000): Column 'mid' cannot be null

-- NOT NULL violations:
INSERT INTO Review VALUES(NULL, '2014-01-19 03:14:07', 3423, 1, 'SPAM!');
-- Fails because name parameter cannot be null
-- ERROR 1048 (23000): Column 'name' cannot be null

-- BEGIN MaxPersonID AND MaxMovieID TABLE VIOLATIONS:
-------------------------------------------------------------------------------
-- CHECK violation:
UPDATE MaxPersonID SET id = 4593;
-- Should theoretically fail because 4593 <> 1+(SELECT MAX(A.id) FROM
-- 											   (SELECT Actor.id AS id 
--										        FROM Actor UNION SELECT Director.id
--																 FROM Director) A
-- Violation would be similar for MaxMovieID.