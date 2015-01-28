						 Simple Movie Database Framework, v. 1.0
							  last updated: Jan 29, 2014


AUTHORS
-------
Ahsan, Prianna
email: prianna@ucla.edu
SID: 704-068-040

Chemelekov, Rossen
email: rchemelekov@gmail.com
SID: 504-216-616

INCLUDED FILES
--------------
queries.sql: Sample valid SQL queries to the database.
violate.sql: Sample invalid SQL queries to the database.
create.sql: Constructs the database framework. Contains table schemas and constraints.
			Actor, Movie, and Director tables are currently referenced by MovieActor, 
			MovieDirector, MovieGenre, and MovieReview tables.
load.sql: Loads data files into framework. Assumes that data files are stored in ../data
		  folder (e.g. assumes that /data folder is one level down from the MySQL data 
		  directory).
query.php: PHP script that accepts user input, queries the database, and displays output in
		   tabular form. Currently does not validate user input prior to querying database.
README.txt: This file.

GENERAL USAGE NOTES
-------------------
The simple Movie Database Framework was designed to support the development of a user updated
database of movies, actors, and directors. Users can enter new movies, actors, and directors
into the database, as well as assign genres and post reviews of movies currently in the database.
Users can also assign actors/directors to movies, thereby allowing the update of cast/production
lists. Currently, a user can enter any input desired into the user interface. This poses a
security risk, and we plan to update the front-end to validate and error-check user input.


FUTURE IMPLEMENTATION PLAN
--------------------------
create.sql: Need to implement constraint or trigger that enforces single tuple integrity
			for MaxMovieID and MaxPersonID tables.
query.php: Need to implement form input validation and error handling.

BUG REPORTING
-------------
Any bugs should be reported to prianna@ucla.edu or rchemelekov@gmail.com. Please note that lack
of error handling and form validation is not a bug, and any errors thrown from entry of invalid
queries should be regarded as working as intended.