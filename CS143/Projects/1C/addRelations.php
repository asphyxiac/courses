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
                <li><a href="#">Actor/Movie</a></li>
                <li><a href="#">Director/Movie</a></li>		
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
            <h2><center>Add Relations</center></h2>
			<p>

<form method="GET">
<center>
<select name="type">
<option value="0">Available operations:</option>
<option value="addActrOrDir" <?php if($_GET["type"] == addActrOrDir) echo "selected=\"selected\""; ?>>Add Actor or Director</option>
<option value="addMovie" <?php if($_GET["type"] == addMovie) echo "selected=\"selected\""; ?>>Add Movie </option>
<option value="addRelation" <?php if($_GET["type"] == addRelation) echo "selected=\"selected\""; ?>>Add Relation</option>
</select>
<input type="submit" value="Open Form" />
</center>
</form>
</font>
<center>
<?php
if($_GET["type"])
{
	$type = $_GET["type"];
	if ($type == "addActrOrDir")
	{ ?>
<p>
<form method="post">
<?php
	if($_GET["type"] == addActrOrDir)
		echo "<h3>Add actor or director: </h3>"; ?>
<p><b>First: </b><input type="text" name="first" maxlength="20"/><br/></p>
<p><b>Last: </b><input type="text" name="last" maxlength="20" /><br/></p>
<p><b>Gender: </b><input type="radio" name="sex" value="Male" checked/> Male
<input type="radio" name="sex" value="Female"/> Female</p>
<p><b>Actor or Director: </b>
<input type="checkbox" name="actcheck"/> Actor
<input type="checkbox" name="dircheck"/> Director </p>
<p><b>Date of Birth: </b>
<select name="dobm"><option value="0"></option>
<?php
 	$month = array( '01' => 'January', '02' => 'February', '03' => 'March', '04' => 'April',
 					'05' => 'May', '06' => 'June', '07' => 'July', '08' => 'August',
 					'09' => 'September', '10' => 'October', '11' => 'November', '12' => 'December');

	foreach ($month as $count => $m) 
		echo "<option value=\"$count\">$m</option>";
 ?>
 </select>
 <select name="dobd"><option value="0"></option>
 <?php for ($i = 01; $i < 32; $i++) echo "<option value=\"$i\">$i</option>"; ?></select>
 <select name="doby"><option value="0"></option>
 <?php for ($i = 2012; $i > 1900; $i--) echo "<option value=\"$i\">$i</option>"; ?></select>
</p>
<p><b>Date of Death (blank if living): </b>
 <select name="dodm"><option value="0"></option>
 <?php 
	foreach ($month as $count => $m) echo "<option value=\"$count\">$m</option>";	?></select>
 <select name="dodd"><option value="0"></option>
 <?php for ($i = 01; $i < 32; $i++) echo "<option value=\"$i\">$i</option>"; ?></select>
 <select name="dody"><option value="0"></option>
 <?php for ($i = 2012; $i > 1900; $i--) echo "<option value=\"$i\">$i</option>"; ?></select>
</p>

<p><input type="submit" value="Add Person" name="clicked"/></p>
</form>
</p>
</center>
<?php } // close adding director/ actor form if statement

 if ($type == "addMovie")
 { ?>
<center>
<p>
<form method="post">
<h3>Add a new movie: </h3>
<input type="hidden" name="clicked" />
<p>Title: <input type="text" maxlength="100" name="title" <?php if(!$_GET["did"]) echo "disabled=\"disabled\""; ?>/><br/></p>
<p>Company: <input type="text" maxlength="50" name="company" <?php if(!$_GET["did"]) echo "disabled=\"disabled\""; ?>/><br/></p>
<p>Year: <input type="text" name="year" <?php if(!$_GET["did"]) echo "disabled=\"disabled\""; ?>/><br/></p>
<p>Director:
</center>
<?php
	if ($_GET["did"] > 1){
		$did = $_GET["did"];
		$db = mysql_connect("localhost", "cs143", "");
		if(!$db) 
		{
			$errmsg = mysql_error($db);
			 print "Connection failed: $errmsg <br />";
			 exit(1);
		}
		mysql_select_db("CS143", $db);

		$direct = "SELECT first, last, id FROM Director WHERE id = $did";
		$result = mysql_query($direct, $db);

		if($result && mysql_num_rows($result) > 0)
		{
			$r = mysql_fetch_row($result);
			echo "<a href='./browseDirectors.php?id=$did'><b>$r[0] $r[1]</a> </b> ";
			echo "(<a href='./addRelations.php?type=addMovie'><i>clear</i></a>)";
		}

	} elseif ($_GET["did"] == 1) 
	{
		echo " <a href='./addRelations.php?type=addMovie'><i>change your mind?</i></a>";
	} 
	else 
	{
?>
<center>
<form method="POST">
<input type="text" name="director" placeholder="Search a name!"/>
<input type="hidden" name="clicked" value="dirsearch"/>
<input type="submit" value="Search!" />
<br>(<a href="./addRelations.php?type=addMovie&did=1"><i>add without director</i></a> )
</p>
</form>
</center>
<?php }

if ($type == "addMovie" && ($_POST["director"]) && ($_POST["clicked"] == "dirsearch"))
{
	$raw = $_POST["director"]; 			
	$firstc = substr($raw, 0, 1); 		
	$shortflag = 0; 					
	$lengthflag = 0; 					

	if ($firstc == "\\") 
	{
		$shortflag = 1;
		$raw = substr($raw, 1);
	}

	$newinput = preg_replace('/\s+/', ' ', $raw); 	
	$input = explode(" ", $newinput); 				

	foreach ($input as $tr)
	{
		if (strlen($tr) < 3)
			$lengthflag = 1;
	}

	$dquery = "SELECT DISTINCT first, last, id FROM Director WHERE first LIKE '%$input[0]%'";
	foreach($input as $d)
	{
		$dquery .= " OR first LIKE '%$d%' OR last LIKE '%$d%'";
	}
	$dquery .= " ORDER BY first, last";

	if (str_replace(" ", "", $newinput) == "")
	{
		echo "<b><p>Invalid input!</p></b>";
	} elseif ($lengthflag == 1 && $shortflag == 0) 
	{

		echo "<b><p>Input must be longer than 3 chars.</p></b>";
		echo "</br> To force search with fewer chars, enter '\\' before your search term.";
	}
	else 
	{
		 $db = mysql_connect("localhost", "cs143", "");
		  if(!$db) 
		  {
			 $errmsg = mysql_error($db);
				print "Connection failed: $errmsg <br />";
				exit(1);
		}
		mysql_select_db("CS143", $db);

		$result = mysql_query($dquery, $db);
		if ($result && mysql_num_rows($result) > 0)
		{
			echo "<p><b>Results:</b> <br/>";
			while ($r = mysql_fetch_row($result))
			{
				echo "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
				echo "<a href='./addRelations.php?type=addMovie&did=$r[2]'>";
				echo "$r[0] $r[1] </a><br/>";
			} echo "</p></div>";
		} 
		else 
		{ 
			echo "No matching directors found." ; 
		}
		mysql_close($db);
	}
}
?>
<center>
<p>MPAA Rating:
<select name="MPAA">
<option value="G">G</option>
<option value="NC-17">NC-17</option>
<option value="PG">PG</option>
<option value="PG-13">PG-13</option>
<option value="R">R</option>
</select></p>
<center>
Genre:</br/>
<?php
$genres = array("Action", "Adult", "Adventure", "Animation", "Comedy", "Crime",	"Documentary", "Drama", 
"Family", "Fantasy", "Horror", "Musical", "Mystery", "Romance", "Sci-Fi", "Short", "Thriller", "War", "Western");

	foreach ($genres as $gen)
	{
		echo "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
		echo "<input type=\"checkbox\" name=\"genre[]\" value=\"$gen\"/> $gen <br/> ";
	}
	echo "</p></div>";
?>
<center>
<input type="hidden" name="clicked" value="23"/>
<p><input type="submit" value="Add Movie" <?php if(!$_GET["did"]) echo "disabled=\"disabled\"";?>/>
<?php if($type == "addMovie" && !$_GET["did"]) {?>
(first <a href="./addRelations.php?type=addMovie&did=1"><i>pick</i></a> a director)
<?php } ?>
</form>
</p>
</center>

<?php
if ($type == "addMovie" && $_GET["did"] && $_POST["title"] && $_POST["company"]
&& $_POST["year"] && $_POST["MPAA"] && is_numeric($_POST["year"]))
{
	$did = $_GET["did"];
	$title = $_POST["title"];
	$company = $_POST["company"];
	$year = $_POST["year"];
	$mpaa = $_POST["MPAA"];
	$temp = array("Action", "Adult", "Adventure", "Animation", "Comedy", "Crime",
		"Documentary", "Drama", "Family", "Fantasy", "Horror", "Musical", "Mystery",
		"Romance", "Sci-Fi", "Short", "Thriller", "War", "Western");
	$genres = array();

	if($year > 2025 || $year < 1900)
	{
		echo "<font color='red'>Year must be between 1900 and 2050.</font> ";
	} 
	else 
	{

	for($i = 0; $i < 19; $i++)
	{
		if(isset($_POST["genre"][$i]))
			array_push($genres, $_POST["genre"][$i]);
	}


		$db = mysql_connect("localhost", "cs143", "");
		if(!$db)
		{
			$errmsg = mysql_error($db);
			print "Connection failed: $errmsg <br />";
			exit(1);
		}
		mysql_select_db("CS143", $db);

		$midquery = "SELECT id FROM MaxMovieID";
		$midsearch = mysql_query($midquery, $db);
		$midfinished = mysql_fetch_row($midsearch);
		$mid = $midfinished[0];
		
		$addm = "INSERT INTO Movie VALUES($mid, '$title', $year, '$mpaa', '$company')";

		if ($did > 1)
			$adddir = "INSERT INTO MovieDirector VALUES ($mid, $did)";
		$update = "UPDATE MaxMovieID SET id=id+1";


		if(mysql_query($addm, $db))
		{
			mysql_query($adddir, $db);
			echo "<p>Successfully added \"$title\" into the database! Thank you! </p>";
			echo "View your movie <a href='./browseMovies.php?id=$mid'>here</a></br> ";


			foreach($genres as $g)
			{
				mysql_query("INSERT INTO MovieGenre VALUES ($mid, '$g')", $db);
			}

			mysql_query($update, $db);

		} else 
		{ 
			echo "Failed to add movie. Next time. "; 
		}
		mysql_close($db);
	}


} elseif ($type == "addMovie" && $_GET["did"] && ($_POST["clicked"]=="23") 
	 && (!$_POST["title"] || !$_POST["company"]
	  || !$_POST["year"] || !$_POST["MPAA"] || !is_numeric($_POST["year"]))) 
{
	echo "<b>";
	if (!$_POST["title"])
		echo "<font color='red'>Please enter a title.</font> <br/>";
	if (!$_POST["company"])
		echo "<font color='red'>Please enter a company.</font> <br/>";
	if (!$_POST["year"])
		echo "<font color='red'>Please enter a year.</font> <br/>";
	if (!$_POST["MPAA"])
		echo "<font color='red'>Please enter an MPAA rating.</font> <br/>";
	if (!is_numeric($_POST["year"]))
		echo "<font color='red'>Invalid year. </font><br/>";
	echo "</b>";
} }

if ($type == "addRelation" && $_GET["mid"])
{
	$mid = $_GET["mid"];

	$db = mysql_connect("localhost", "cs143", "");
	if(!$db) 
	{
		$errmsg = mysql_error($db);
		print "Connection failed: $errmsg <br />";
		exit(1);
	}
	mysql_select_db("CS143", $db);

	$qTitle = "SELECT title FROM Movie WHERE id = $mid";
	$title = mysql_query($qTitle, $db);

	if ($title && mysql_num_rows($title) > 0)
	{
		$r = mysql_fetch_row($title);
		echo "<h4>Movie: $r[0]</h4>";
	} 
	else 
	{ 
		echo "No movies matched this ID."; 
	}
	mysql_close($db);
}

if ($type == "addRelation" && $_GET["aid"])
{
	 $aid = $_GET["aid"];
	 
	$db = mysql_connect("localhost", "cs143", "");
	if(!$db) 
	{
		$errmsg = mysql_error($db);
		print "Connection failed: $errmsg <br />";
		 exit(1);
	}
	mysql_select_db("CS143", $db);

	$qName = "SELECT first, last FROM Actor WHERE id = $aid";
	$title = mysql_query($qName, $db);

	if ($title && mysql_num_rows($title) > 0)
	{
		$r = mysql_fetch_row($title);
		echo "<h4>Actor: $r[0] $r[1]</h4>";
	}
	else 
	{
		echo "No matching actors with such ID.";
	}
	 mysql_close($db);
 }

if ($type == "addRelation" && $_GET["did"])
{
	$did = $_GET["did"];
	$db = mysql_connect("localhost", "cs143", "");
	if(!$db) 
	{
		$errmsg = mysql_error($db);
		print "Connection failed: $errmsg <br />";
		 exit(1);
	}
	mysql_select_db("CS143", $db);

	$qName = "SELECT first, last FROM Director WHERE id = $did";
	$title = mysql_query($qName, $db);

	if ($title && mysql_num_rows($title) > 0)
	{
		$r = mysql_fetch_row($title);
		echo "<h4>Director: $r[0] $r[1]</h4>";
	} 
	else 
	{ 
		echo "No matching directors with such ID."; 
	}
	 mysql_close($db);
 }

 if ($type == "addRelation" && $_GET["mid"] && $_GET["aid"]){ ?>
 <center>
<form method = "POST">
<h4>Role : <input type="text" name="role" maxlength="50"/></h4>
<p>Add relationship?
<input type="hidden" name="select" value="addam"/>
<input type="hidden" name="disable" value="yes"/>
<input type="submit" value="Add relation!"
<?php
	if($_POST["disable"]=="yes" && $_POST["role"]) echo "disabled=\"disabled\""; ?>/></p>
</form>
<center>
<?php }

if ($type == "addRelation" && $_POST["select"] == "addam" && $_POST["role"])
{
	$mid = $_GET["mid"]; 
	$aid = $_GET["aid"]; 
	$temprole = $_POST["role"];

	$db = mysql_connect("localhost", "cs143", "");
	 if(!$db) 
	 {
		$errmsg = mysql_error($db);
		 print "Connection failed: $errmsg <br />";
		 exit(1);
	 }

	mysql_select_db("CS143", $db);

	if (str_replace(" ", "", $temprole) == "")
	{
		echo "<font color='red'>Enter a valid role for this actor.</font>";
	} 
	else 
	{
		$role = mysql_real_escape_string($temprole);
		$relation = "INSERT IGNORE INTO MovieActor VALUES ($mid, $aid, '$role')";
		if (mysql_query($relation, $db))
		{
			echo "<p><b><u>Sucessfully added actor to movie! </b></u>";
			echo "<br/>View your new relation <a href='./browseMovies.php?id=$mid'>here</a></p>";
		} 
		else 
		{ 
			echo "<b>Failed to add actor to movie.</b>"; 
		}
	}
	mysql_close($db);
} 
elseif ($type == "addRelation" && $_POST["select"] == "addam" && !$_POST["role"])
{
	echo "<b><font color='red'>Please enter a role.</font></b>";
}

 if ($type == "addRelation" && $_GET["mid"] && $_GET["did"]){ ?>
 <center>
<form method = "POST">
<input type="hidden" name="select" value="adddm"/>
<input type="hidden" name="disable" value="yes"/>
<input type="submit" value="Add relation!"
<?php 
	if($_POST["disable"]=="yes") echo "disabled=\"disabled\""; ?>/></p>
</form>
<center>
<?php } 

if ($type == "addRelation" && $_POST["select"] == "adddm")
{
	$mid = $_GET["mid"];
	$did = $_GET["did"];

	$db = mysql_connect("localhost", "cs143", "");
	 if(!$db) 
	 {
		$errmsg = mysql_error($db);
		 print "Connection failed: $errmsg <br />";
		 exit(1);
	 }
	mysql_select_db("CS143", $db);
	$relation = "INSERT IGNORE INTO MovieDirector VALUES ($mid, $did)";

	if (mysql_query($relation, $db))
	{
		echo "<p><b><u>Sucessfully added director to movie! </b></u>";
		echo "<br/>View your new relation <a href='./browseMovies.php?id=$mid'>here</a></p>";
	} 
	else 
	{ 
		echo "<b><p>Failed to add movie.</b></p>"; 
	}
	mysql_close($db);
}

if($_POST["smovie"] && ($_POST["but"] == "mov"))
{
	$raw = $_POST["smovie"]; 
	$newinput = preg_replace('/\s+/', ' ', $raw); 
	$words = explode(" ", $newinput);

	$db = mysql_connect("localhost", "cs143", "");
	if(!$db) 
	{
		$errmsg = mysql_error($db);
		 print "Connection failed: $errmsg <br />";
		 exit(1);
	}
	mysql_select_db("CS143", $db);

	$input = array();
	foreach ($words as $w)
	{
		array_push($input, mysql_real_escape_string($w));
	}

	$mquery= "SELECT title, id FROM Movie WHERE title LIKE '%$input[0]%'";

	foreach($input as $t)
	{
		$mquery .= " OR title LIKE '%$t%'";
	}
	$mquery .= " ORDER BY title";

	$result = mysql_query($mquery, $db);

	if($result && mysql_num_rows($result) > 0)
	{
		echo "<br/><b><p>Movies: </p></b><p>";

		while($r = mysql_fetch_row($result))
		{
			echo "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
			if ($_GET["aid"])
			{
				$aid = $_GET["aid"];
				echo "<a href = './addRelations.php?type=addRelation&aid=$aid&mid=$r[1]'>";
			} elseif ($_GET["did"])
			{
				$did = $_GET["did"];
				echo "<a href = './addRelations.php?type=addRelation&did=$did&mid=$r[1]'>";
			} 
			else
				echo "<a href = './addRelations.php?type=addRelation&mid=$r[1]'>";
			echo "" .$r[0]. "</a><br/>";
		}
		echo "</p></div>";
	} 
	else 
	{ 
		echo "<br/><b>No movie matched \"$raw\".</b>";
	}
	mysql_close($db);
}

if($_POST["sactor"] && ($_POST["but"] == "act"))
{
	$raw = $_POST["sactor"]; 
	$newinput = preg_replace('/\s+/', ' ', $raw); 
	$words = explode(" ", $newinput);

	$db = mysql_connect("localhost", "cs143", "");
	 if(!$db) 
	 {
		$errmsg = mysql_error($db);
		 print "Connection failed: $errmsg <br />";
		 exit(1);
	 }
	mysql_select_db("CS143", $db);

	$input = array();
	foreach ($words as $w)
	{
		array_push($input, mysql_real_escape_string($w));
	}

	$aquery= "SELECT DISTINCT first, last, id FROM Actor WHERE first LIKE '%$input[0]%'";

	foreach($input as $t)
	{
		$aquery .= " OR first LIKE '%$t%' OR last LIKE '%$t%'";
	}
	$aquery .= " ORDER BY first, last";
	$result = mysql_query($aquery, $db);

	if($result && mysql_num_rows($result) > 0)
	{
		echo "<br/><b>Actors: </b><p>";
		while($r = mysql_fetch_row($result))
		{
			echo "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
			if ($_GET["mid"])
			{
				$mid = $_GET["mid"];
				echo "<a href = './addRelations.php?type=addRelation&mid=$mid&aid=$r[2]'>";
			}
			else
				echo "<a href = './addRelations.php?type=addRelation&aid=$r[2]'>";
			echo "$r[0] $r[1]</a><br/>";
		}
		echo "</p></div>";
	} 
	else
	{ 
		echo "<br/><b>No actor found with search \"$raw\"</b>"; 
	}
	mysql_close($db);
}

if($_POST["sdirector"] && ($_POST["but"] == "dir"))
{
	$raw = $_POST["sdirector"];
	$newinput = preg_replace('/\s+/', ' ', $raw);
	$words = explode(" ", $newinput);

	$db = mysql_connect("localhost", "cs143", "");
	 if(!$db)
	 {
		$errmsg = mysql_error($db);
		 print "Connection failed: $errmsg <br />";
		 exit(1);
	 }
	mysql_select_db("CS143", $db);

	$input = array();
	foreach ($words as $w)
	{
		array_push($input, mysql_real_escape_string($w));
	}

	$aquery= "SELECT DISTINCT first, last, id FROM Director WHERE first LIKE '%$input[0]%'";

	foreach($input as $t)
	{
		$aquery .= " OR first LIKE '%$t%' OR last LIKE '%$t%'";
	}
	$aquery .= " ORDER BY first, last";


	$result = mysql_query($aquery, $db);

	if($result && mysql_num_rows($result) > 0)
	{
		echo "<br/><b>Directors: </b><p>";
		while($r = mysql_fetch_row($result))
		{
			echo "&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
			if ($_GET["mid"])
			{
				$mid = $_GET["mid"];
				echo "<a href = './addRelations.php?type=addRelation&mid=$mid&did=$r[2]'>";
			} 
			else
				echo "<a href = './addRelations.php?type=addRelation&did=$r[2]'>";
			echo "$r[0] $r[1]</a><br/>";
		}
		echo "</p></div>";
	} 
	else 
	{ 
		echo "<br/><b>No director found with search \"$raw\"</b>"; 
	}
	mysql_close($db);
}

if ($type == "addRelation") { ?>
<center>
<p><form method="POST">
Movies: 
<input type="text" name="smovie" placeholder="Find a movie" />
<input type="hidden" name="but" value="mov" />
<input type="submit" value="Search" />
</form>
</p>

<p><form method="POST">
Actors: 
<input type="text" name="sactor" placeholder="Find an actor"/>
<input type="hidden" name="but" value="act" />
<input type="submit" value="Search" />
</form>
</p>

<p><form method="POST">
Directors: 
<input type="text" name="sdirector" placeholder="Find a director"/>
<input type="hidden" name="but" value="dir" />
<input type="submit" value="Search" />
</form>
</p>
</center>
<?php }

if($type == "addActrOrDir" && $_POST["first"] &&
	$_POST["last"] && $_POST["doby"] && $_POST["dobm"] && $_POST["dobd"] &&
	(isset($_POST["actcheck"]) || isset($_POST["dircheck"])))
{
	$firsttemp = $_POST["first"]; 			
	$lasttemp = $_POST["last"]; 			
	$sex = $_POST["sex"]; 					
	$aflag = isset($_POST["actcheck"]); 	
	$dflag = isset($_POST["dircheck"]); 	

	$dobd = $_POST["dobd"]; 				
	$dobm = $_POST["dobm"]; 				
	$doby = $_POST["doby"]; 				
	$dob = "$doby-$dobm-$dobd"; 			

	$dodd = $_POST["dodd"]; 				
	$dodm = $_POST["dodm"]; 				
	$dody = $_POST["dody"]; 				
	$dod = "$dody-$dodm-$dodd"; 			
	$dodflag = 0; 							
	$success = 0;
	$addactor = ""; 						
	$adddirector = "";

	if ($dodd > 0 && $dodm > 0 && $dody > 9)
	{
		$dodflag = 1; 
	}


	if (str_replace(" ", "", $firsttemp) == "")
	{ 
		echo "<font color='red'>First name is required.</font> ";
	} elseif (str_replace(" ", "", $lasttemp) == "")
	{ 
		echo "<font color='red'>Last name is required.</font> ";
	} 
	elseif (($dody>0 && ($dodm==0 || $dodd==0)) ||
				$dodm>0 && ($dody==0 || $dodd==0) ||
				$dodd>0 && ($dody==0 || $dodm==0))
	{
		echo "<font color='red'>Invalid date of death, if still alive leave blank.</font> ";
	} 
	elseif ($dody != 0 && $dodm != 0 && $dodd != 0 &&
			(strtotime($dod) < strtotime($dob)))
	{ 
		echo "<font color='red'>Invalid date of death.</font> ";
	}
	elseif (($doby%4!=0 && $dobm == 2 && $dobd > 28) ||
			  ($doby%4==0 && $dobm == 2 && $dobd > 29)) 
	{
		echo "<font color='red'>$dobm - $dobd - $doby is not a valid date.</font>";
	} 
	elseif (($dobm==4 || $dobm==6 || $dobm==9 || $dobm==11) && $dobd > 30) 
	{
		echo "<font color='red'>$dobm - $dobd - $doby is not a valid date.</font>";
	} 
	elseif (($dody%4!=0 && $dodm == 2 && $dodd > 28) ||
			  ($dody%4==0 && $dodm == 2 && $dodd > 29)) 
	{
		echo "<font color='red'>$dodm - $dodd - $dody is not a valid date</font>";
	} 
	elseif (($dodm==4 || $dodm==6 || $dodm==9 || $dodm==11) && $dodd > 30)
	{
		echo "<font color='red'>$dodm - $dodd - $dody is not a valid date.</font>";
	} 
	else
	{
		$db = mysql_connect("localhost", "cs143", "");
		if(!$db) 
		{
			$errmsg = mysql_error($db);
			print "Connection failed: $errmsg <br />";
			exit(1);
		}
		mysql_select_db("CS143", $db);

		$first = mysql_real_escape_string($firsttemp);
		$last = mysql_real_escape_string($lasttemp);
		$pidquery = "SELECT id FROM MaxPersonID";
		$pidsearch = mysql_query($pidquery, $db);
		$pidfinished = mysql_fetch_row($pidsearch);
		$pid = $pidfinished[0];

		if ($dodflag == 0)
			$addactor = "INSERT INTO Actor VALUES ($pid, '$last', '$first', '$sex', '$dob', NULL)";
		else
			$addactor = "INSERT INTO Actor VALUES ($pid, '$last', '$first', '$sex', '$dob', '$dod')";
		if($dodflag == 0)
			$adddirector = "INSERT INTO Director VALUES ($pid, '$last', '$first', '$dob', NULL)";
		else
			$adddirector = "INSERT INTO Director VALUES ($pid, '$last', '$first', '$dob', '$dod')";

		echo "<p>";
		if($aflag)
		{
			if (mysql_query($addactor, $db))
			{
				echo "$firsttemp $lasttemp successfully added to actors. ";
				echo "<a href='./browseActors.php?id=$pid'>View</a> addition.<br/>";
				$success = 1;
			} 
			else 
			{ 
				echo "Failed to add actor. ";
			}
		}

		if($dflag)
		{
			if (mysql_query($adddirector, $db))
			{
				echo "$firsttemp $lasttemp successfully added to directors. ";
				echo "<a href='./browseDirectors.php?id=$pid'>View</a> addition.<br/> ";
				$success = 1;
			}
			else 
			{ 
				echo "Failed to add director. ";
			}
		}

		if($success==1)
			mysql_query("UPDATE MaxPersonID SET id=id+1", $db);
		echo "</p>";
		mysql_close($db);
	}
}

elseif ($type=="addActrOrDir" && $_POST["clicked"] && (!$_POST["first"] || !$_POST["last"] ||
		$_POST["doby"]=="0" || $_POST["dobm"]=="0" || $_POST["dobd"]=="0" ||
		(!isset($_POST["actcheck"]) && !isset($_POST["dircheck"]))))
{
	echo "<b>";
	if (!$_POST["first"])
		echo "<font color='red'>Enter first name. </font><br/>";
	if (!$_POST["last"])
		echo "<font color='red'>Enter a last name. </font><br/>";
	if (!isset($_POST["actdir"][0]) && !isset($_POST["actdir"][1]))
		echo "<font color='red'>Choose to add actor or director.</font> <br/>";
	if ($_POST["doby"]=="0" || $_POST["dobm"]=="0" || $_POST["dobd"]=="0")
		echo "<font color='red'>Enter a valid date of birth.</font><br/>";
	echo "</b>";
}
?>
<center>
<p>
<form action="./addRelations.php?type=" method="GET">
<input type="hidden" name="type" value="<?php echo $_GET["type"] ?>";/>
<input type="submit" value="Reset form" />
</form></p>
</center>
<?php } // type if statement (very first one) ?>

</div>
</body>
</html>
