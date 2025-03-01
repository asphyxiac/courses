-- Movie table
LOAD DATA LOCAL INFILE './data/movie.del' INTO TABLE Movie 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"';

-- Actor table
LOAD DATA LOCAL INFILE './data/actor1.del' INTO TABLE Actor 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"';
LOAD DATA LOCAL INFILE './data/actor2.del' INTO TABLE Actor 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"';
LOAD DATA LOCAL INFILE './data/actor3.del' INTO TABLE Actor 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"';

-- Director table
LOAD DATA LOCAL INFILE './data/director.del' INTO TABLE Director 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"';

-- MovieGenre table
LOAD DATA LOCAL INFILE './data/moviegenre.del' INTO TABLE MovieGenre 
FIELDS TERMINATED BY ',' 
OPTIONALLY ENCLOSED BY '"';

-- MovieDirector table
LOAD DATA LOCAL INFILE './data/moviedirector.del' INTO TABLE MovieDirector 
FIELDS TERMINATED BY ',';

-- MovieActor table
LOAD DATA LOCAL INFILE './data/movieactor1.del' INTO TABLE MovieActor 
FIELDS TERMINATED BY ','
OPTIONALLY ENCLOSED BY '"';
LOAD DATA LOCAL INFILE './data/movieactor2.del' INTO TABLE MovieActor 
FIELDS TERMINATED BY ','
OPTIONALLY ENCLOSED BY '"';

INSERT INTO MaxPersonID VALUES(69000);
INSERT INTO MaxMovieID VALUES(4750);
