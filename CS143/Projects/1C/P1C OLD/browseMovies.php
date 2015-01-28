<!DOCTYPE html>
<html>
<head>
<title>Browse Movies</title>
<link href="./Site.css" rel="stylesheet">
</head>
<?php include("./Header.php"); ?>

<div id="main">
<h1>Browse Movies</h1>

<?php
// get input
if($_GET["id"])
{
	// get movie id
	$input = $_GET["id"];

	// establish connection
	$db = mysql_connect("localhost", "cs143", "");
	if(!$db)
	{
		$errmsg = mysql_error($db);
		print "Connection failed: $errmsg <br>";
		exit(1);
	}
	mysql_select_db("CS143", $db);

	// look at movie
	$mov = "SELECT title, rating, year, company
			FROM Movie M
			WHERE id = '$input'";

	// look for director
	$dir = "SELECT first, last, dob, id
			FROM Director D, MovieDirector MD
			WHERE MD.mid = '$input' AND MD.did = D.id";

	// look for actors
	$act = "SELECT first, last, dob, id, role
			FROM Actor A, MovieActor MA
			WHERE MA.mid = '$input' AND MA.aid = A.id
			ORDER BY first, last";

	// genre
	$gen = "SELECT genre
			FROM MovieGenre
			WHERE mid = '$input'";

	// review
	$rev = "SELECT name, time, mid, rating, comment
			FROM Review
			WHERE mid = '$input'";

	// ratings
	$rat = "SELECT AVG(rating)
			FROM Review
			WHERE mid = '$input'
			GROUP BY mid";

	$movie = mysql_query($mov, $db);
	$director = mysql_query($dir, $db);
	$actor = mysql_query($act, $db);
	$genre = mysql_query($gen, $db);
	$review = mysql_query($rev, $db);
	$rating = mysql_query($rat, $db);

	if (($movie && mysql_num_rows($movie) > 0) && $director && $genre)
	{
		$movieInfo = mysql_fetch_row($movie);
		echo '<h2>Information about "';
		echo "$movieInfo[0]";
		echo '"</h2>';
		echo "<p>";
		echo "Title: <b>$movieInfo[0]</b><br>";
		echo "Production Company: <b>$movieInfo[3]</b><br>";
		echo "MPAA Rating: <b>$movieInfo[1]</b><br>";
		echo "Director: <b>";
		if (mysql_num_rows($director) > 0)
		{
			$i = 0;
			while ($movDir = mysql_fetch_row($director))
			{
				echo "<a href='./browseActors.php?id=$d[3]'>$movDir[0] $movDir[1]</a>";
				if($i < mysql_num_rows($director) - 1)
				{
					echo ", ";
					$i++;
				}
			}
			echo "<br>";
		}
		else
		{
			echo "<form action=\"./addActor.php?type=5\" method=\"GET\">";
			echo "<input type=\"hidden\" name=\"type\" value =\"5\"/>";
			echo "<input type=\"hidden\" name=\"mid\" value =\"$input\"/>";
			echo "<input type=\"submit\" value=\"Add a director\"/>";
			echo "</form><p>";
		}
		echo "</b>";
		echo "Genre: <b>";
		$i = 0;
		if (mysql_num_rows($genre) > 0)
		{
			while ($genreInfo = mysql_fetch_row($genre))
			{
				echo "$genreInfo[0]";
				if($i < mysql_num_rows($genre) - 1)
				{
					echo ", ";
					$i++;
				}
			}
		}
		else
			echo " --unspecified--";
		echo "</b><br>";
		echo "Average rating: <b> ";
		if ($rating && mysql_num_rows($rating) > 0)
		{
			$avgRating = mysql_fetch_row($rating);
			if ($avgRating[0] > 0)
				echo "$avgRating[0] stars.</b>";
			else
				echo "No ratings for this film yet.</b>";
		}
		else
			echo "No ratings yet! </b>";
		echo "<br><br>";

		echo "<b>Cast:</b><br>";
		if ($actor && mysql_num_rows($actor) > 0)
		{
			while ($actorRow = mysql_fetch_row($actor))
			{
				// for each element in that row
				echo "<a href = './browseActors.php?id=$actorRow[3]'>";
				for($i=0; $i<2; $i++)
					echo " $actorRow[$i]";
				echo "</a> ";
				echo "as \"$actorRow[4]\"<br>";
			}
		}
		else
		{
			echo "There is no information about ";
			echo "$movieInfo[0]";
			echo "'s cast yet.<br>";
		}

		echo "</p>";
		// add an actor
		echo "<form action=\"./addActor.php\" method=\"GET\">";
		echo "<input type=\"hidden\" name=\"type\" value =\"5\"/>";
		echo "<input type=\"hidden\" name=\"mid\" value =\"$input\"/>";
		echo "<input type=\"submit\" value=\"Add an actor\"/>";
		echo "</form>";

		echo '<br><b>Reviews for "';
		echo "$movieInfo[0]";
		echo '":</b>';
		if ($review && mysql_num_rows($review) > 0)
		{
			while($row = mysql_fetch_row($review))
			{
				echo "<p><b>$row[0]</b> rated this movie <b>$row[3]</b> stars";
				echo " on $row[1] <br>";
				echo "<b>Comment:</b> $row[4]</p>";
			}
		}
		else
			echo "<br>There are no reviews for $movieInfo[0] yet.<br>" ;
	}
	else
		echo "<b>Invalid movie!</b>";

?>
<form action="./addComment.php?id=" method="GET">
<p>
<input type="hidden" name="id" value ="<?php echo $input; ?>"/>
<input type="submit" value="Add a review" /></p>
</form>
<?php

	// DO NOT FORGET to close the database
	mysql_close($db);
}
?>

<?php
// show results of movies that start with the letter
if($_GET["title"]){
	$titleLetter = $_GET["title"];

	// establish connection
	$db = mysql_connect("localhost", "cs143", "");
	if(!$db) {
		$errmsg = mysql_error($db);
		print "Connection failed: $errmsg <br>";
		exit(1);
	}
	mysql_select_db("CS143", $db);

	$query = "SELECT title, id
			  FROM Movie
			  WHERE title LIKE '$titleLetter%'
			  ORDER BY title";

	echo "<h2>Movies starting with letter <font color='red'><u>$titleLetter</u></font></h2>";
	$result = mysql_query($query, $db);
	echo "</p>";

	if ($result && mysql_num_rows($result) > 0)
	{
		echo "<h3>Matches:</h3>";

		while($row = mysql_fetch_row($result))
		{
			echo "============[ <a href = './browseMovies.php?id=$row[1]'>";
			echo "$row[0] ";
			echo "</a>]<br>";
		}
		echo "</p>";
	}
	else
		echo "No movies found matching criteria.<br>";

	// DO NOT FORGET to close the database
	mysql_close($db);
}
?>

<?php
echo "<h3>Find movies by Title:</h3>";
$letters = array(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z);

foreach ($letters as $eachLetter)
{
	echo "[ <a href=./browseMovies.php?title=$eachLetter>$eachLetter</a> ";
	if($letter != Z)
		echo "]";
}
echo "<br><br>";
?>

</div>
<?php include("./Footer.php"); ?>
</body>
</html>
