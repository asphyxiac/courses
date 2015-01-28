-- Give me the names of all the actors in the movie 'Die Another Day'. 
-- Please also make sure actor names are in this format:  
-- <firstname> <lastname>   (seperated by single space). 
SELECT CONCAT(first, ' ', last) AS 'Cast of "Die Another Day":'
FROM Actor actr, Movie mov, MovieActor movActr
WHERE actr.id = movActr.aid
  AND mov.id = movActr.mid
  AND mov.title = 'Die Another Day';
  
-- Give me the count of all the actors who acted in multiple movies. 
SELECT COUNT(*) AS 'Number of actors in multiple movies:'
FROM (SELECT COUNT(mid)
      FROM Actor actr, MovieActor movActr
      WHERE actr.id = movActr.aid
      GROUP BY id
      HAVING COUNT(mid) > 1) actr;
      
-- All movies Jack Nicholson has been in.
SELECT CONCAT(title, ' as ', role) AS 'Jack Nicholson Films and Role:'
FROM Actor actr, Movie mov, MovieActor movActr
WHERE actr.id = movActr.aid
  AND mov.id = movActr.mid
  AND actr.first = 'Jack'
  AND actr.last = 'Nicholson';

-- All movies containing the term 'Punk'  
SELECT title, year FROM Movie WHERE title LIKE '%Punk%';

-- All actors who have been in a movie with Kevin Bacon.
-- i.e., Actors with a Bacon number of 1.
SELECT CONCAT(first, ' ', last) AS 'Actors with a Bacon number of 1:'
FROM Actor actr, Movie mov, MovieActor movActr, (SELECT movI.id
	  FROM Actor act, Movie movI, MovieActor movA
	  WHERE act.id = movA.aid
	    AND movI.id = movA.mid
	    AND act.first = 'Kevin'
  	    AND act.last = 'Bacon') bacon
WHERE actr.id = movActr.aid
  AND actr.first <> 'Kevin'
  AND actr.last <> 'Bacon'
  AND movActr.mid = bacon.id
  AND mov.id = bacon.id;


 