-- Remove previous tables. Tables must be removed in such a manner
-- as to preserve foreign key/referential integrity. MySQL will not
-- allow deletion Actor table prior to deletion of MovieActor table.
DROP TABLE IF EXISTS MovieGenre;
DROP TABLE IF EXISTS MovieDirector;
DROP TABLE IF EXISTS MovieActor;
DROP TABLE IF EXISTS Review;
DROP TABLE IF EXISTS MaxPersonID;
DROP TABLE IF EXISTS MaxMovieID;
DROP TABLE IF EXISTS Movie;
DROP TABLE IF EXISTS Actor;
DROP TABLE IF EXISTS Director;


-- Create Movie table.
-- Primary Key constraints: Must have id.
-- Referential Integrity constraints: id is a referenced attribute
-- and is unique and never NULL.
-- Check constraints: id > 0, year > 0.
-- Other constraints: Title, year can never be NULL.
-- Using INNODB storage engine
CREATE TABLE Movie(id INT NOT NULL, 
		   title VARCHAR(100) NOT NULL, 
		   year INT NOT NULL, 
		   rating VARCHAR(10), 
		   company VARCHAR(50), 
		   PRIMARY KEY(id),
		   CHECK(id > 0 AND year > 0) )
		   ENGINE = INNODB;

-- Create Actor table.
-- Primary Key: Must have id.
-- Referential Integrity: id is a referenced attribute, unique, and never NULL.
-- Check: id > 0, dob > 0.
-- Other: Every actor needs a DOB and at least a first name 
-- (first name only actors exist e.g., Madonna, Prince).
-- Using INNODB storage engine
CREATE TABLE Actor(id INT NOT NULL, 
		   last VARCHAR(20),
		   first VARCHAR(20) NOT NULL,
		   sex VARCHAR(6), 
		   dob DATE NOT NULL, 
		   dod DATE, 
		   PRIMARY KEY(id),
		   CHECK(id > 0 AND dob > 0) )
		   ENGINE = INNODB;

-- Create Director table
-- Primary Key: must have id.
-- Referential Integrity: id is a referenced attribute, unique, and never NULL.
-- Check:  dob > 0, id > 0.
-- Other: Every director needs a DOB and at least a first name.
-- Using INNODB storage engine
CREATE TABLE Director(id INT NOT NULL, 
		      last VARCHAR(20),
		      first VARCHAR(20) NOT NULL,
		      dob DATE NOT NULL, 
		      dod DATE, 
		      PRIMARY KEY(id),
		      CHECK(id > 0 AND dob > 0) )
		      ENGINE = INNODB;

-- Create MovieGenre table
-- Referential Integrity: Using foreign key mid referencing Movie(id).
-- CASCADE commands carry over changes from referenced relations.
-- Using INNODB storage engine
CREATE TABLE MovieGenre(mid INT NOT NULL, 
		   	genre VARCHAR(20), 
			FOREIGN KEY(mid) REFERENCES Movie(id)
			ON DELETE CASCADE
			ON UPDATE CASCADE)
			ENGINE = INNODB;

-- Create MovieDirector table
-- Referential Integrity: Foreign key mid referencing Movie(id), Director(id).
-- CASCADE commands carry over changes from referenced relations.
-- Using INNODB storage engine
CREATE TABLE MovieDirector(mid INT NOT NULL, 
		   	   did INT NOT NULL, 
			   FOREIGN KEY(mid) REFERENCES Movie(id) 
   			   ON DELETE CASCADE ON UPDATE CASCADE,
			   FOREIGN KEY(did) REFERENCES Director(id)
		   	   ON DELETE CASCADE ON UPDATE CASCADE)
			   ENGINE = INNODB;

-- Create MovieActor table, using foreign keys
-- Referential Integrity: Foreign key mid referencing Movie(id), Actor(id).
-- CASCADE commands carry over changes from referenced relations.
-- Using INNODB storage engine
CREATE TABLE MovieActor(mid INT NOT NULL, 
		   	aid INT NOT NULL,
		   	role VARCHAR(50),
			FOREIGN KEY(mid) REFERENCES Movie(id)
			ON DELETE CASCADE ON UPDATE CASCADE,
			FOREIGN KEY(aid) REFERENCES Actor(id)
			ON DELETE CASCADE ON UPDATE CASCADE)
			ENGINE = INNODB;

-- Create Review table
-- Referential Integrity: Foreign key mid (never NULL) referencing Movie(id)
-- Checks: 0<= rating <=5.
-- Other: All entries must have name, rating, movie ID, and timestamp. This is
-- to ensure proper chronological display/to avoid non-movie related comments.
-- Using INNODB storage engine
CREATE TABLE Review(name VARCHAR(20) NOT NULL,
		    time TIMESTAMP NOT NULL,
		    mid INT NOT NULL,  
		    rating INT NOT NULL,  
		    comment VARCHAR(500), 
		    FOREIGN KEY(mid) REFERENCES Movie(id)
			ON DELETE CASCADE ON UPDATE CASCADE,
		    ENGINE = INNODB;

-- Issues encountered with enforcing integrity of single valued tables
-- MaxPersonID, MaxMovieID:
-- 1. How to enforce one tuple limit/prevent insertion of additional tuples?
--   	Possible to enforce via PHP scripting?
--		Alternatively, use triggers (upon insert or update) to enforce?


-- Create MaxPersonID table
-- DEFAULT keyword ensures that the id attribute is never NULL.
-- Check statement makes sure that the id is never incremented
-- more than 1 digit at a time.
CREATE TABLE MaxPersonID(id INT DEFAULT 69000,
CHECK (id = 1+(SELECT MAX(A.id) FROM
			  (SELECT Actor.id AS id
				  FROM Actor
				  UNION SELECT Director.id
			  	  FROM Director) A ) ) );

-- Create MaxMovieID table
-- DEFAULT keyword ensures that the id attribute is never NULL.
-- Check statement makes sure that the id is never incremented
-- more than 1 digit at a time.
CREATE TABLE MaxMovieID(id INT DEFAULT 4750,
CHECK (id = 1+(SELECT MAX(Movie.id) FROM Movie)) );