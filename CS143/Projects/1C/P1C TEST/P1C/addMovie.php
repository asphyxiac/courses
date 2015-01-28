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
            <h2>Update Database</h2>
			<p>
<?php
$errors  = array();
$values  = array();
$genre = array();
$errmsg  = '';

require_once 'php/login.php';
$db_server = mysql_connect($db_hostname, $db_username, $db_password);

if (!$db_server) die("Unable to connect to MySQL: " . mysql_error());

mysql_select_db($db_database, $db_server) or die("Unable to select database: " . mysql_error());

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
	
	
    foreach($_POST as $key => $value) {
        	$values[$key] = trim(stripslashes($value)); // basic input filter
    }

    if (check_input($values['movTitle']) == false)
        $errors['movTitle'] = "Must enter a movie title.";
    

    if (check_input($values['movYear']) == false) {
        $errors['movYear'] = "Must enter a release year.";
    } else if (verifyDate($values['movYear']) == false) {
        $errors['movYear'] = "Invalid year.";
    }
	
	if (isset($_POST['genre']))
	{
		foreach($_POST['genre'] as $var => $temp)
		{
			$genre[$var] = trim(stripslashes($temp));
			if ( check_input($temp) == false )
			{
		        	$errors[$temp] = "Invalid genre.";
			}

		}
	}
}
		
				
	if (sizeof($errors) == 0) 
			{
				if( isset($_POST['movTitle']) &&
					isset($_POST['movYear']) )
				{	
					$movTitle = get_post('movTitle'); 
					$movYear = get_post('movYear');
					$movCompany = get_post('company');
					$movRating = get_post('rating');
					
	
					$get_ID = "SELECT max(id) FROM Movie";
					$result = mysql_query($get_ID);
					$row = mysql_fetch_row($result);
					$movID = $row[0]+1;
					
					$query = "INSERT INTO Movie VALUES" .
					"('$movID', '$movTitle', '$movYear', '$movRating', '$movCompany')";
					
					if (!mysql_query($query, $db_server))
						 echo "INSERT failed: $query<br />" . 
						 mysql_error() . "<br /><br />";
	 				else
	 					{	
	 						$increment_ID = "UPDATE MaxMovieID SET id = '$movID' WHERE id >= 0";
	 						if(!mysql_query($increment_ID, $db_server))
	 							echo mysql_error() . "<br /><br />";
	 					}
					
					if (isset($_POST['genre']))
					{
						$movGenre = $genre;
						foreach($movGenre as $key => $value)
						{
							$movGenre[$key] = mysql_real_escape_string($value);
							$genre_query = "INSERT INTO MovieGenre VALUES" .
							"('$movID', '$movGenre[$key]')";
						
							if (!mysql_query($genre_query, $db_server))
								 echo "INSERT failed: $genre_query<br />" . 
								 mysql_error() . "<br /><br />";
						}
					}
			
				}
				$query = "SELECT * FROM Movie WHERE id = '$movID'"; 
				$result = mysql_query($query);
				
				$genre_query = "SELECT genre FROM MovieGenre WHERE mid = '$movID'";
				$genre_result = mysql_query($genre_query);
				

				if (!$result) die ("Database access failed: " . mysql_error()); 

				$rows = mysql_num_rows($result);
				for ($j = 0 ; $j < $rows ; ++$j)
				{
					$row = mysql_fetch_row($result); 
					echo 'The following data have been recorded: ' . '<br />';
					echo 'Movie ID: ' . $row[0] . '<br />';
					echo 'Title: ' . $row[1] . '<br />';
					echo 'Year: ' . $row[2] . '<br />';
					echo 'MPAA Rating: ' . $row[3] . '<br />';
					echo 'Production Company: ' . $row[4] . '<br />';	
					echo 'The following genres have been associated: ' . '<br />';	
				}	
				
				$rows = mysql_num_rows($genre_result);
				for ($j = 0 ; $j < $rows ; ++$j)
				{
					$row = mysql_fetch_row($genre_result); 
					if ($row[0] == '')
						echo 'No associated genres.' . '<br />';
					else
						echo $row[0] . '<br />';
				}	
					
			mysql_close($db_server);
	        $values = array();
	    } 
		else {
	        // one or more errors
	        foreach($errors as $error) {
	            $errmsg .= $error . '<br />';
	        }
	    }
?>

            <p><a name ="AddMovie">
				<form method="post" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF'], ENT_QUOTES, "utf-8"); ?>">
					<?php if ($errmsg != ''): ?>
					 <p style="color: red;"><b>Please correct the following errors:</b><br />
					 <?php echo $errmsg; ?>
					 </p>
					 <?php endif; ?>
				    <fieldset>
				        <legend>Add Movie</legend>

					<div>
		                <label for="title">Title*</label>
		                <input name="movTitle" type="text" value="<?php echo htmlspecialchars(@$values['movTitle']) ?>" required>
		            </div>

		            <div>
		                <label for="year">Release Year*</label>
		                <input name="movYear" type="text" value="<?php echo htmlspecialchars(@$values['movYear']) ?>"required>
		            </div>


			         <div>
		                <label for="Rating">MPAA Rating</label>
		                <select name="rating" required>
		                    <option>G</option>
		                    <option>PG</option>
		                    <option>PG-13</option>
		                    <option>R</option>
		                    <option>NC-17</option>
		                    <option>NR</option>
			                </select>
			           </div>
						
				        <div>
				            <label for="company">Production Company</label>
							<input name="company" type="text" value="<?php echo htmlspecialchars(@$values['company']) ?>">
				        </div>
						
						<?php
						if (isset($_POST['genre']))
						{
							foreach($_POST['genre'] as $selectedGenre)
								$selected[$selectedGenre] = "checked";
						}
						?>
						
						<div>
							<INPUT TYPE="checkbox" NAME="genre[]" VALUE="Action"  <?php echo $selected['Action'] ?> />Action
							<INPUT TYPE="checkbox" NAME="genre[]" VALUE="Adult"  <?php echo $selected['Adult'] ?> />Adult
							<INPUT TYPE="checkbox" NAME="genre[]" VALUE="Adventure" <?php echo $selected['Adventure'] ?> />Adventure
							<INPUT TYPE="checkbox" NAME="genre[]" VALUE="Animation" <?php echo $selected['Animation'] ?> />Animation
							<INPUT TYPE="checkbox" NAME="genre[]" VALUE="Comedy" <?php echo $selected['Comedy'] ?> />Comedy
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Crime'] ?> VALUE="Crime" />Crime
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Documentary'] ?> VALUE="Documentary" />Documentary
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Drama'] ?> VALUE="Drama" />Drama
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Family'] ?> VALUE="Family" />Family
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Fantasy'] ?> VALUE="Fantasy" />Fantasy
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Horror'] ?> VALUE="Horror" />Horror
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Musical'] ?> VALUE="Musical" />Musical
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Mystery'] ?> VALUE="Mystery" />Mystery
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Romance'] ?> VALUE="Romance" />Romance
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Sci-Fi'] ?> VALUE="Sci-Fi" />Sci-Fi
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Short'] ?> VALUE="Short" />Short
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Thriller'] ?> VALUE="Thriller" />Thriller
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['TV Show'] ?> VALUE="TV Show" />TV Show
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['War'] ?> VALUE="War" />War
							<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Western'] ?> VALUE="Western" />Western
						</div>
						
				        <input type="submit" name="submitMovie" value="Submit" />
						<br><br>
						* indicates a required field.
				
				    </fieldset>
				</form>
			</p>
			
        </div>
    </div>
</div>







</body>
</html>



<?php



function verifyDate($date)
{
	$input = (int)$date;
	    if($input > 1000 && $input < 2100)
	    {
			return true;
		}
		else
			return false;
}

function check_input($input) {
    if (strlen($input) == 0) {
        return false;
    } else {
        // TODO: other checks?

        return true;
    }
}

function get_post($var)
{
	if(strlen($_POST[$var]) == 0)
		return false;
	else
		return mysql_real_escape_string($_POST[$var]);
} 

?>