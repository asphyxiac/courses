						 		MovieBase Web Interface, v. 1.0
							  	  last updated: Feb 12, 2014


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
index.html: 	Main landing page for the MovieBase web interface.

INSERTION UTILITIES:
addActor.php: 	Allows the user to add an actor the database via an HTML form. PHP scripts are
			  	used to dynamically parse and validate input like names and dates. Errors such as 
				missing required fields will not allow the form to be submitted until the errors 
				are fixed. The form should repopulate with previously entered values if errors 
				are found.
addComment.php: Allows the user to leave a review for a movie in the database. The user can
 				either search for the movie within the page. If the movie is found in the 
				database, a PHP script will auto-populate the form with the appropriate movie ID 
				and movie title values. Reviews must be at least 5 characters and at most 500 
				characters long.
addDirector.php:Allows the user to add a director to the database via an HTML form. PHP scripts
				are used to dynamically parse and validate input like names and dates. Errors 
				such as missing required fields will not allow the form to be submitted until the 
				errors are fixed. The form should repopulate with previously entered values if
				errors are found.
addMovie.php:	Allows the user to add a movie to the database via an HTML form. PHP scripts are
			  	used to dynamically parse and validate input like title and release year. The user
				can select a movie genre (optional) from a number of checkboxes. Errors such as 
				missing required fields will not allow the form to be submitted until the errors 
				are fixed. The form should repopulate with previously entered values if errors 
				are found.
				
UPDATE UTILITIES:
addRelations.php: Allows the user to link movies, actors, and directors in the database from a
				  single page.

SELECTION UTILITIES:
browseActors.php: Allows the user to browse people (actors or directors) in the database.
				  Clicking on a person brings up their info page, which contains all movies
				  they are associated, their roles, and personal information.
browseDirectors.php: Allows the user to browse people in the database. Identical functionality to browse
		     actors page.
browseMovies.php: Allows the user to browse movies in the database. Clicking on a movie brings up 
				  its info page, which displays all information about the movie, including MPAA 
				  rating, cast, associated genres, and any review information.
Search.php: 	  Allows the user to search for a specific person (actors or directors) or movie
				  in the database. Spaces are interpreted as OR, not AND.
				  Clicking on a person brings up their info page, which contains all movies
				  they are associated, their roles, and personal information.
				  Clicking on a movie brings up its info page, which displays all information 
				  about the movie, including MPAA rating, cast, associated genres, and any review 
				  information.
				  
README.txt: This file.

GENERAL USAGE NOTES AND FEATURES
--------------------------------
The MovieBase web interface was designed to function as a front end for the simple MovieDatabase
framework implemented in Project 1B.  

The following features were implemented, please see included files list for details specific to
individual pages and forms.

Input:
- Users are able to add actors and/or directors with reasonable names with required fields for 
  name, date of birth and gender. Names can contain apostrophes and hyphens, so the name "D'Angelo 
  Acosta-Dunham" would be accepted as valid.
- Users can add movies to the databases, with title, year, genre, and rating requirements.
- Users can add comments to movies.
- Users can add actor-to-movie and director-to-movie relations.

Browsing and Searching:
- When an actor is looked up, there are links to the movies that actor has been in. Same for 
  directors.
- When a movie is looked up, there are links to the cast, as well as options with links to add 
  actor, director, and/or review to the movie.
- Browsing the database for actors/directors can be done through a keyword search or by using
  first name and further narrowed down by using the last name.
- Upon look up of an actor or director, information about the actor or director is presented, such 
  as Filmography, Date of Birth, and Date of Death.
- Lastly, the user can search the database for an actor, director or movie using keywords giving 
  results for all matches, exactly as directed by the specification and results are given in
  alphabetic order.

COLLABORATION NOTES
-------------------
Rossen Chemelekov:	Implemented first draft functionaly of php scripts for the search.php, 					   browseActors.php, browseMovie.php and addRelation pages. Implemented majority
					of final PHP scripting for addRelation.php, browseActors.php, and 
					browseMovies.php. Contributed to README.txt. Audited test cases and
					debugged/tested addDirector.php, addActor.php, addMovie.php.

Prianna Ahsan:		Designed, implemented, and tested html/CSS styles. Implemented first draft and 
					final functionality of addDirector.php, addActor.php, addMovie.php, 
					addComment.php. Ran final test cases using Selenium. Debugged/tested/made 
					structural edits to search.php, browseActors.php, browseMovie.php, and 
					addRelation pages. Contributed to and finalized formatting for README.txt.
					
Major Challenges:	Time management is always a challenge. This project was especially challenging 
					with respect to time constraints because of the proximity of its due date to
					midterms week. Additionally, both partners (like many students) have very busy 
					schedules and are taking a full course load. Both partners are also commuters, 
					and live > 40 min from campus. Prianna is currently involved in several 
					research projects with her lab on campus and Rossen works 20 hours a week in 
					addition to his coursework. Communication, when not carried out in person, 
					occured via instant messaging, text messages, and e-mails.
					
					
FUTURE IMPLEMENTATION PLAN
--------------------------
Additional security checks and form validation is needed across all scripts.

BUG REPORTING
-------------
Any bugs should be reported to prianna@ucla.edu or rchemelekov@gmail.com.