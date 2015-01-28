<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta name="description" content="MovieBase, a CS143 production">

<title>MovieBase &ndash;A CS143 Production &ndash;</title>

<link rel="stylesheet" href="css/style.css">
</head>

<body>
<div id="layout">
    <!-- Menu toggle -->
    <a href="#menu" id="menuLink" class="menu-link">
        <!-- Hamburger icon -->
        <span></span>
    </a>
    <div class="menu">
	    <div id="sidebar">
            <a class="menu-heading" href="#">Update</a>
            <ul>
				<li><a href="addActor.php">Actor</a></li>
				<li><a href="addDirector.php">Director</a></li>
                <li><a href="addMovie.php">Movie</a></li>
                <li><a href="addRelations.php">Actor/Movie</a></li>
                <li><a href="addRelations.php">Director/Movie</a></li>		
            </ul>
            <a class="menu-heading" href="#">Browse</a>
            <ul>
                <li><a href="browseActors.php">People</a></li>
                <li><a href="browseMovies.php">Movies</a></li>
                <li><a href="Search.php">Search</a></li>
				
                <li><a href="addComment.php">Comments</a></li>
				<li><a href="index.html">Home</a></li>
            </ul>
        </div>
    </div>

    <div class="main-content">
        <div class="header">
            <h1>MovieBase</h1>
            <h2>The Dynamic Film Database!</h2>
        </div>
	    <div class="content">
            <h2>Browse Actors and Directors</h2>
			<p>

				<?php
				// get input
				if($_GET["id"])
				{
					$input = $_GET["id"]; // id of actor

					// connect to the database or give error
					$db = mysql_connect("localhost", "cs143", "");
					if(!$db)
					{
						$errmsg = mysql_error($db);
						print "Connection failed: $errmsg <br>";
						exit(1);
					}
					mysql_select_db("CS143", $db);

					// run queries on database
					// get info on actor and director with matching id
					$queryActr = "SELECT first, last, dob, dod, sex
								  FROM Actor
								  WHERE id = '$input'";
					$queryDirctr = "SELECT first, last, dob, dod
							   		FROM Director
							   		WHERE id = '$input'";

					// get info on matching actor movies
					$queryMA = "SELECT title, role, mid, year
							    FROM MovieActor MA, Movie M
							    WHERE MA.aid = '$input' AND MA.mid = M.id
							    ORDER BY year DESC";

					// info on matching director movies
					$queryMD = "SELECT title, mid, year
								 FROM MovieDirector MD, Movie M
								 WHERE MD.did = '$input' AND MD.mid = M.id
								 ORDER BY year DESC";

					$act = mysql_query($queryActr, $db);
					$dir = mysql_query($queryDirctr, $db);
					$mov = mysql_query($queryMA, $db);
					$movDir = mysql_query($queryMD, $db);

					if (($act && mysql_num_rows($act) <= 0))
						echo "Invalid actor id.";
					else
					{
						// pull up actor info
						if (($act && mysql_num_rows($act) > 0) || ($dir && (mysql_num_rows($dir) > 0)))
							{
								$row = mysql_fetch_row($act);

								echo "<h2>Results for <u>$row[0] $row[1]</u>:</h2><p>";
								echo "Name: <b>$row[0] $row[1]</b><br/>";
								echo "Gender: <b>$row[4]</b><br/>";
								$dob = date("F d, Y", strtotime($row[2]));
								echo "Date of Birth: <b>$dob</b><br/>";
								if ($row[3] != NULL)
								{
									$dod = date("F d, Y", strtotime($row[3]));
									echo "Date of Death: <b>$dod</b><br/>";
								}
								else
									echo "Date of Death: <b>[Still alive]</b>";
						}
						else
							echo "Invalid actor/director id.";

						echo "<br><br><b>Actor in:</b><br>";

						if ($mov && mysql_num_rows($mov) > 0)
						{
							while ($filmInfo = mysql_fetch_row($mov))
							{
								// for each element in that row
								echo "<a href = './browseMovies.php?id=$filmInfo[2]'>";
								echo "$filmInfo[0]</a> as \"$filmInfo[1]\"";
								echo " - " .$filmInfo[3]. ".<br>";
							}
						}
						else
							echo "No data on movies $row[0] $row[1] acted in.<br>";

						echo "<br><b>Director of: </b><br>";
						if ($movDir && mysql_num_rows($movDir) > 0)
						{
							while ($movInfo = mysql_fetch_row($movDir))
							{
								echo "<a href = './browseMovies.php?id=$movInfo[1]'>";
								echo "$movInfo[0]</a> - $movInfo[2]<br>";
							}
						}
						else
							echo "No data on movies $row[0] $row[1] directed.<br>";
					}

					// DO NOT FORGET to close the database connection
					mysql_close($db);
				}

				// matches by first name letter
				if($_GET[first] && !$_GET[last])
				{
					$firtsNameLetter = $_GET[first];

					// connect
					$db = mysql_connect("localhost", "cs143", "");
					if(!$db)
					{
						$errmsg = mysql_error($db);
						print "Connection failed: $errmsg <br>";
						exit(1);
					}
					mysql_select_db("CS143", $db);

					$queryActr = "SELECT first, last, id
							  	  FROM Actor
							  	  WHERE first LIKE '$firtsNameLetter%'
							  	  ORDER BY first";

					echo "<h2>Actor/Director whose First Name starts with <font color='red'><u>$firtsNameLetter</u></font></h2>";
					$result = mysql_query($queryActr, $db);

					// matches by last name letter
					echo "<b>Filter by Last Name: </b><br>";
					$lastNameLetter = array(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z);
					foreach($lastNameLetter as $lastName)
						echo "[ <a href=./browseActors.php?first=$firtsNameLetter&last=$lastName>$lastName</a> ]";
					echo "<br>";

					if ($result && mysql_num_rows($result) > 0)
					{
						echo "<h3>Choose from:</h3>";
						while($row = mysql_fetch_row($result))
						{
							echo "============[ ";
							echo "<a href = './browseActors.php?id=$row[2]'>";
							for($j=0; $j<2; $j++)
								echo "$row[$j] ";
							echo "</a>]<br>";
						}
						echo "<br>";
					}
					else
						echo "<b>No actors/directors found.</b>";

					// DO NOT FORGET to close the database
					mysql_close($db);
				}
				elseif ($_GET[first] && $_GET[last])
				{
					$firstNameLetter = $_GET[first];
					$lastNameLetter = $_GET[last];

					// connect
					$db = mysql_connect("localhost", "cs143", "");
					if(!$db)
					{
						$errmsg = mysql_error($db);
						print "Connection failed: $errmsg <br>";
						exit(1);
					}
					mysql_select_db("CS143", $db);

					// get the actors
					$queryActr = "SELECT first, last, id
								  FROM Actor
								  WHERE first LIKE '$firstNameLetter%' AND last LIKE '$lastNameLetter%'
								  ORDER BY first, last";

					echo "<h2>Actor/Director whose First Name starts with <font color='red'><u>$firstNameLetter</u></font><br>";
					echo "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
					echo "and Last Name begins with <font color='red'><u>$lastNameLetter</u></font></h2>";

					$result = mysql_query($queryActr, $db);

					// filter by last name
					echo "<p><b>Change last name filter: </b><br>";
					$lastNameLetters = array(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z);
					foreach($lastNameLetters as $letters)
						echo "[ <a href=./browseActors.php?first=$firstNameLetter&last=$letters>$letters</a> ]";
					echo "</p>";

					// list actors
					if ($result && mysql_num_rows($result) > 0)
					{
						echo "<h3>Matches:</h3>";

						while($row = mysql_fetch_row($result))
						{
							echo "============[ <a href = './browseActors.php?id=$row[2]'>";
							for($j=0; $j<2; $j++)
								echo "$row[$j] ";
							echo "</a>]<br>";
						}
						echo "</p>";
					}
					else
						echo "No actors/directors found matching criteria.<br>";

					// DO NOT FORGET to close the database
					mysql_close($db);
				}

				// persistent navigation used to search for a different name
				echo "<br><h3>Find Actor/Director by First Name:</h3>";
				$firstNameLetter = array(A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z);

				foreach ($firstNameLetter as $firstName)
				{
					echo "[ <a href=./browseActors.php?first=$firstName>$firstName</a> ";
					if($letter != Z)
						echo "]";
				}
				echo "<br><br>";
				?>

</p>
</div>
</div>
</body>
</html>
