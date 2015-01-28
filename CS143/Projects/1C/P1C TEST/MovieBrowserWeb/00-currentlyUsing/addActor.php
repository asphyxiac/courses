<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<meta name="description" content="MovieBase, a CS143 production">

    <title>MovieBase &ndash;A CS143 Production &ndash;</title>

<link rel="stylesheet" href="css/pure.css">
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
                <li><a href="./addActor.php">Actor</a></li>
                <li><a href="./addDirector.php">Director</a></li>
                <li><a href="./AddMovie.php">Movie</a></li>
                <li><a href="I4.html#ActorMovie">Actor/Movie</a></li>
                <li><a href="I4.html#DirectorMovie">Director/Movie</a></li>
            </ul>
            <a class="menu-heading" href="#">Browse</a>
            <ul>
                <li><a href="./browseActors.php">Actors</a></li>
                <li><a href="./browseMovies.php">Movies</a></li>
                <li><a href="./Search.php">Search</a></li>
                <li><a href="#">Comments</a></li>
				<li><a href="./index.html">Home</a></li>
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
$errmsg  = '';

require_once 'php/login.php';
$db_server = mysql_connect($db_hostname, $db_username, $db_password);

if (!$db_server) die("Unable to connect to MySQL: " . mysql_error());

mysql_select_db($db_database, $db_server) or die("Unable to select database: " . mysql_error());

if ($_SERVER['REQUEST_METHOD'] == 'POST')
{
    foreach($_POST as $key => $value)
    {
        $values[$key] = trim(stripslashes($value)); // basic input filter
    }

    if (check_input($values['actFirstName']) == false)
        $errors['actFirstName'] = "Must enter actor's first name.";

    if (check_input($values['actDOB']) == false)
    {
        $errors['actDOB'] = "Must enter DOB.";
    } else if (verifyDate($values['actDOB']) == false)
    {
        $errors['actDOB'] = "Invalid date format.";
    }

	if ((verifyDate($values['actDOD']) == false) && (check_input($values['actDOD']) != NULL))
	        $errors['actDOD'] = "Invalid date format.";

	if (sizeof($errors) == 0)
	{
				if( isset($_POST['submitActor']) &&
					isset($_POST['actFirstName']) &&
					isset($_POST['actDOB']) &&
					get_post('actFirstName') )
				{
					$actFirstName = get_post('actFirstName');
					$actLastName = get_post('actLastName');
					$actDOB = get_post('actDOB');
					$actDOD = get_post('actDOD');
					$actSex = get_post('actSex');

					$get_ID = "SELECT max(pid) FROM ( SELECT id as pid FROM Actor UNION SELECT id as pid FROM Director) as mid";
					$result = mysql_query($get_ID);
					$row = mysql_fetch_row($result);
					$actID = $row[0]+1;

					$query = "INSERT INTO Actor VALUES" .
					"('$actID', '$actLastName', '$actFirstName', '$actSex', '$actDOB', '$actDOD')";

					if (!mysql_query($query, $db_server))
						 echo "INSERT failed: $query<br />" .
						 mysql_error() . "<br /><br />";

					else
					{
						$increment_ID = "UPDATE MaxPersonID SET id = '$actID' WHERE id >= 0";
						if(!mysql_query($increment_ID, $db_server))
							echo mysql_error() . "<br /><br />";
					}
				}
				$query = "SELECT * FROM Actor WHERE id = '$actID'";
				$result = mysql_query($query);

				if (!$result) die ("Database access failed: " . mysql_error());

				$rows = mysql_num_rows($result);
				for ($j = 0 ; $j < $rows ; ++$j)
				{
					$row = mysql_fetch_row($result);
					echo 'The following data has been recorded: ' . '<br />';
					echo 'Actor: ' . $row[0] . '<br />';
					echo 'Last Name: ' . $row[1] . '<br />';
					echo 'First Name: ' . $row[2] . '<br />';
					echo 'Gender: ' . $row[3] . '<br />';
					echo 'DOB: ' . $row[4] . '<br />';
					if ($row[5] == 0000-00-00)
						echo 'DOD: [Still alive] <br />';
					else
						echo 'DOD: ' . $row[5] . '<br />';
				}
			mysql_close($db_server);
	        $values = array();
	    }
		else
		{
	        // one or more errors
	        foreach($errors as $error)
	        {
	            $errmsg .= $error . '<br />';
	        }
	    }
	}
?>
			<a name ="AddActor">
				<form method="post" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF'], ENT_QUOTES, "utf-8"); ?>">
					<?php if ($errmsg != ''): ?>
					 <p style="color: red;"><b>Please correct the following errors:</b><br />
					 <?php echo $errmsg; ?>
					 </p>
					 <?php endif; ?>
				    <fieldset>
				        <legend>Add Actor</legend>
							<div>
				                <label for="firstName">First Name *</label>
				                <input name="actFirstName" type="text" value="<?php echo htmlspecialchars(@$values['actFirstName']) ?>" required>
				            </div>

				            <div>
				                <label for="lastName">Last Name</label>
				                <input name="actLastName" type="text" value="<?php echo htmlspecialchars(@$values['actLastName']) ?>">
				            </div>

				            <div>
				                <label for="DOB">Date of Birth (YYYY-MM-DD) *</label>
				                <input name="actDOB" type="text" value="<?php echo htmlspecialchars(@$values['actDOB']) ?>"required>
				            </div>

				            <div>
				                <label for="DOD">Date of Death (YYYY-MM-DD)</label>
				                <input name="actDOD" type="text" value="<?php echo htmlspecialchars(@$values['actDOD']) ?>">
				            </div>

				            <div>
				                <label for="sex">Gender *</label>
				                <select name="actSex" required>
				                    <option>Male</option>
				                    <option>Female</option>
				                    <option>Unknown</option>
				                </select>
				            </div>

				        <input type="submit" name="submitActor" value="Submit"/>
						<br>
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
function date_create_from_format( $dformat, $dvalue )
  {

    $schedule = $dvalue;
    $schedule_format = str_replace(array('M', 'Y', 'm', 'd', 'H', 'i', 'a'), array('%b', '%Y', '%m', '%d', '%H', '%M', '%p'), $dformat);
    // %Y, %m and %d correspond to date() is Y m and d.
    // %I corresponds to H, %M to i and %p to a
    $ugly = strptime($schedule, $schedule_format);
    $ymd = sprintf(
        // This is a format string that takes six total decimal
        // arguments, then left-pads them with zeros to either
        // 4 or 2 characters, as needed
        '%04d-%02d-%02d %02d:%02d:%02d',
        $ugly['tm_year'] + 1900,  // This will be "111", so we need to add 1900.
        $ugly['tm_mon'] + 1,      // This will be the month minus one, so we add one.
        $ugly['tm_mday'],
        $ugly['tm_hour'],
        $ugly['tm_min'],
        $ugly['tm_sec']
    );
    $new_schedule = new DateTime($ymd);

   return $new_schedule;
  }

function verifyDate($date)
{
   return preg_match( '#^(?P<year>\d{2}|\d{4})([- /.])(?P<month>\d{1,2})\2(?P<day>\d{1,2})$#', $date, $matches )
           && checkdate($matches['month'],$matches['day'],$matches['year']);
}

function check_input($input)
{
    if (strlen($input) == 0)
    {
        return false;
    } else
    {
        // TODO: other checks?

        return true;
    }
}

function get_post($var)
{
	if(strlen($var) == 0)
		return false;
	else
		return mysql_real_escape_string($_POST[$var]);
}
?>