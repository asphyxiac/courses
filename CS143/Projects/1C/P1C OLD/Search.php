<!DOCTYPE html>
<html>
<head>
<title>Search</title>
<link href="./Site.css" rel="stylesheet">
</head>

<?php include("./Header.php"); ?>

<div id="main">
<h1>Enter the name of a movie or a person: </h1>

<?php
// get input
if($_GET["search"] && ($_GET["clicked"]=="yes")){
	$rawinput = $_GET["search"]; 			// unsanitized input
	$firstc = substr($rawinput, 0, 1); 		// get first char
	$shortflag = 0; 						// flag for short search
	$lengthflag = 0; 						// flag for length

	if ($firstc == "\\") {
		$shortflag = 1;
		$rawinput = substr($rawinput, 1);
	}

	// sanitize
	$newinput = preg_replace('/\s+/', ' ', $rawinput);	// replace spaces
	$words = explode(" ", $newinput); 					// separate words

	// check size > 2, set length flag if <=2
	foreach ($words as $w){
		if (strlen($w) < 3)
			$lengthflag = 1;
	}

	// check if input was empty space
	if (str_replace(" ", "", $newinput) == "")
		echo "Invaild input!";
	elseif ($lengthflag == 1 && $shortflag == 0)
	{
		// if short flag not set and length = 1 char
		echo "Input is too short. To force search with this input, type '\\' before search term.";
	}
	else
	{
		echo '<h2>Results for "';
		echo $rawinput;
		echo '": </h2>';

		// connect to database or display error
		$db = mysql_connect("localhost", "cs143", "");
		if(!$db) {
			$errmsg = mysql_error($db);
			print "Connection failed: $errmsg <br />";
			exit(1);
		}
		mysql_select_db("CS143", $db);

		// store useful tables
		$dbname = array(Actor, Director, Movie);
		$queries = array();

		// use mysql_real_escape_string for every word
		$input = array();
		foreach ($words as $w){
			array_push($input, mysql_real_escape_string($w));
		}

		// search tables
		$queries[0] = "SELECT DISTINCT first, last, dob, id FROM Actor WHERE first LIKE '%$input[0]%'";
		$queries[1] = "SELECT DISTINCT first, last, dob, id FROM Director WHERE first LIKE '%$input[0]%'";
		$queries[2] = "SELECT DISTINCT title, id, year FROM Movie WHERE title LIKE '%$input[0]%'";

		// append
		foreach($input as $v){
			$queries[0] .= " OR first LIKE '%$v%' OR last LIKE '%$v%'";
			$queries[1] .= " OR first LIKE '%$v%' OR last LIKE '%$v%'";
			$queries[2] .= " OR title LIKE '%$v%'";
		}

		// order
		$queries[0] .= " ORDER BY first, last";
		$queries[1] .= " ORDER BY first, last";
		$queries[2] .= " ORDER BY title";

		$a = 0;
		foreach($dbname as $type)
		{
			echo "<b>Matches in $type:</b><br>";
			$result = mysql_query($queries[$a], $db);

			$j = 0;
			if ($result && mysql_num_rows($result)>0)
			{
				while ($row = mysql_fetch_row($result))
				{
					if ($type == Movie)
						echo "<a href = './browseMovies.php?id=$row[1]'>";
					elseif ($type == Actor)
						echo "<a href = './browseActors.php?id=$row[3]'>";
					else
						echo "<a href = './browseDirectors.php?id=$row[3]'>";

					// which to output
					if ($type != Movie)
					{
						$dob = date("F d, Y", strtotime($row[2]));
						echo "$row[0] $row[1]</a>  Born: $dob";
					}
					else
						echo "$row[0]</a> - $row[2]";
					echo "<br>";
				}
				//echo "</p>";
				echo "<br>";
			}
			else
				echo "No $type found containing \"$rawinput\".<br><br>";

			// search next
			$a++;
		}

		// DO NOT FORGET to close database
		mysql_close($db);
	}
}
else
{
	if ($_GET["clicked"]=="yes")
		echo "Input is required for search.";
}
?>

<p><form method="GET">
New search:
<input type ="hidden" name="clicked" value="yes">
<input type="text" name="search" placeholder="Enter search term here">
<input type="submit" value="Find it!">
</form>
</p>


</div>
<?php include("./Footer.php"); ?>
</body>
</html>
