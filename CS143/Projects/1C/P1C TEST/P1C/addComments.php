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
				<li><a href="addActor.php">Actor</a></li>
				<li><a href="addDirector.php">Director</a></li>
                <li><a href="addMovie.php">Movie</a></li>
                <li><a href="I4.html#ActorMovie">Actor/Movie</a></li>
                <li><a href="I4.html#DirectorMovie">Director/Movie</a></li>
				
				
            </ul>
            <a class="menu-heading" href="#">Browse</a>
            <ul>
                <li><a href="browseActors.php">People</a></li>
                <li><a href="browseMovies.php">Movies</a></li>
                <li><a href="search.php">Search</a></li>
				
                <li><a href="addComments.php">Comments</a></li>
				<li><a href="index.php">Home</a></li>
            </ul>
			
			
        </div>
    </div>

    <div class="main-content">
        <div class="header">
            <h1>MovieBase</h1>
            <h2>The Dynamic Film Database!</h2>
        </div>
	    <div class="content">
            <h2>Add Movie Review</h2>
		
<?php
$errors  = array();
$values  = array();
$errmsg  = '';
$mov_ID = '';
$mov_title ='';

require_once 'php/login.php';
$db_server = mysql_connect($db_hostname, $db_username, $db_password);

if (!$db_server) die("Unable to connect to MySQL: " . mysql_error());

mysql_select_db($db_database, $db_server) or die("Unable to select database: " . mysql_error());

if( isset($_GET['checkTitle'])
{
	$title = trim(stripslashes($_GET['checkTitle']));
	$title = htmlentities($title);
	$title = mysql_real_escape_string($title);
	$mov_query = "SELECT id, title FROM Movie WHERE title like '$title'";
	
	if (!mysql_query($mov_query, $db_server))
		 echo "INSERT failed: $query<br />" . 
		 mysql_error() . "<br /><br />";
	else
	 {
	 	$mov_result = mysql_query($mov_query);
		$mov_row = mysql_fetch_row($mov_result);
	 	$mov_ID = $mov_row[0];
	 	$mov_title = $mov_row[1];
	 }
		 
}

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    foreach($_POST as $key => $value) {
        $values[$key] = trim(stripslashes($value)); // basic input filter
    }

    if (check_input($values['revName']) == false)
        $errors['revName'] = "Must enter a name.";
    

    if (check_input($values['movTitle']) == false) {
        $errors['movTitle'] = "Must search for a valid title.";
    } 
	
    validateStr($values['revComment'], 'revComment', 5, 500);
			
	if (sizeof($errors) == 0) 
			{
				if( isset($_POST['revName']) &&
					isset($_POST['movTitle']) &&
					isset($_POST['movID']) &&
					isset($_POST['revComment'])
				{	
					$revName = get_post('revName'); 
					$movID = get_post('movID'); 
					$comment = check_review($_POST['revComment']); 
					$rating = get_post('revRating');
		
					$query = "INSERT INTO Review VALUES" .
					"('$revName', UNIX_TIMESTAMP(), '$movID', '$rating', '$comment')";
	
					if (!mysql_query($query, $db_server))
						 echo "INSERT failed: $query<br />" . 
						 mysql_error() . "<br /><br />";
					else
						echo "Success! Review recorded. <br />";
			
				}
			
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
	}
?>
<p>
			<a name ="AddReview">
				<form method="GET" action="">
				<fieldset>
                <label for="checkTitle">Movie Title</label>
                <input name="checkTitle" type="text" value="Enter title">
		        <input type="submit" name="submitTitle" value="Check Title"/>
				</fieldset>
				</form>
				
				<form method="post" action="<?php echo htmlspecialchars($_SERVER['PHP_SELF'], ENT_QUOTES, "utf-8"); ?>">
					<?php if ($errmsg != ''): ?>
					 <p style="color: red;"><b>Please correct the following errors:</b><br />
					 <?php echo $errmsg; ?>
					 </p>
					 <?php endif; ?>
				    <fieldset>
				        <legend>Add Review</legend>
							<div>
				                <label for="Name">Name*</label>
				                <input name="revName" type="text" value="<?php echo htmlspecialchars(@$values['revName']) ?>" required>
				            </div>

				            <div>
				                <label for="Title">Movie Title</label>
				                <input name="movTitle" type="text" value="<?php echo $mov_title ?>" readonly>
				            </div>
														

				            <div>
				                <label for="mid">Movie ID*</label>
				                <input name="mID" type="text" value="<?php echo $mov_ID) ?>"readonly>
				            </div>

				            <div>
				                <label for="comment">Review*</label>
				                <input name="revComment" type="text"  maxlength="500" value="<?php echo htmlspecialchars(@$values['revComment']) ?>">
				            </div>

				            <div>
				                <label for="sex">Rating*</label>
				                <select name="revRating" required>
				                    <option>1</option>
				                    <option>2</option>
				                    <option>3</option>
				                    <option>4</option>
				                    <option>5</option>
				                </select>
				            </div>

				        <input type="submit" name="submitReview" value="Submit"/>
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




function check_review($review)
{
	$review = trim(stripslashes($review));
	$review = htmlentities($review);
	$review = mysql_real_escape_string($review);
	$values['revComment'] = $review;
	return $review;
}

function validateStr($postVal, $postName, $min = 5, $max = 500) {
    if(strlen($postVal) < intval($min)) {
      $this->setError($postName, ucfirst($postName)." must be at least {$min} characters long.");
    } else if(strlen($postVal) > intval($max)) {
      $this->setError($postName, ucfirst($postName)." must be less than {$max} characters long.");
    }
  }
  
function setError($element, $message) {
      $this->errors[$element] = $message;
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
	if(strlen($var) == 0)
	return false;
	else
	return mysql_real_escape_string($_POST[$var]);
} 
?>