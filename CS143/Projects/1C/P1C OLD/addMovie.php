<!DOCTYPE html>
<html>
<head>
<title>Add Movie</title>
<link href="./Site.css" rel="stylesheet">
<style>
.error {color: #FF0000;}
</style>
</head>
<?php include("./Header.php"); ?>

<div id="main">
<h1>Update the Movie Database</h1>
<h2>Add a new movie:</h2>

<?php
// error fields
$titleErr = $companyErr = $yearErr = $ratingErr = $genreErr = "";
$title = $company = $year = $rating = $genre = "";

if ($_SERVER["REQUEST_METHOD"] == "GET")
{
// collect title and check if it is legit
	if (empty($_GET["title"]))
		$titleErr = "Movie title is required.";
	else
	{
		$title = test_input($_GET["title"]);
    	 // check if title contains allowed chars
     	if (!preg_match("/^[a-zA-Z0-9'.,!\?\- ]*$/",$title))
       		$titleErr = "Invalid character in title name.";
	}

// repeat for company
	if (empty($_GET["company"]))
		$companyErr = "Company is required.";
	else
	{
		$company = test_input($_GET["company"]);
   		if (!preg_match("/^[a-zA-Z0-9'.,!\?\- ]*$/",$company))
   			$companyErr = "Invalid character in company name.";
   	}

// store year
	if (empty($_GET["year"]))
		$yearErr = "Year is required.";
	else
		$year = test_input($_GET["year"]);

// store rating
	if (empty($_GET["rating"]))
		$ratingErr = "Rating is required.";
	else
		$rating = test_input($_GET["rating"]);

// genre
	$selectedNum = $_GET["genre"];
	if (empty($_GET["genre"]))
		$genreErr = "Movie genre is required.";
	else
	{
    	$num = count($selectedNum);
  	}
}

// clean up input
function test_input($data)
{
     $data = trim($data);
     $data = stripslashes($data);
     $data = htmlspecialchars($data);
     return $data;
}

// error message if required fields empty
?>
<p><span class="error">* required field.</span></p>

<form method="get" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>">
Movie Title: <input type="text" name="title" value="<?php echo $title;?>">
			 <span class="error">* <?php echo $titleErr;?></span>
<br><br>
Company: <input type="text" name="company" value="<?php echo $company;?>">
		 <span class="error">* <?php echo $companyErr;?></span>
<br><br>

<?php echo "Year: ";
echo '<select name="year">'; echo '<option value=""></option>';
for($i = date('Y'); $i >= date('Y', strtotime('-100 years')); $i--)
{
	//echo "<option value='$i'>$i</option>";
	echo "<option value='$i'>$i";
	if($year == $i)
	{
		$sel = 'selected="selected"';
		echo '<option '.$sel.' value ="'.$i.'">'.$i.'</option>';
	}
	echo '</option>';
}
echo '</select> ';
?>
<span class="error">* <?php echo $yearErr;?></span>
<br><br>

<?php echo "MPAA Rating: "; ?>
<td><select name ="rating" value="$rating">
<option value=''></option>
<option value='G' <?php if($rating == 'G') echo 'selected="selected"'; ?> >G</option>
<option value='PG-13' <?php if($rating == 'PG-13') echo 'selected="selected"'; ?> >PG-13</option>
<option value='PG-17' <?php if($rating == 'PG-17') echo 'selected="selected"'; ?> >PG-17</option>
<option value='NC-17' <?php if($rating == 'NC-17') echo 'selected="selected"'; ?> >NC-17</option>
<option value='R' <?php if($rating == 'R') echo 'selected="selected"'; ?> >R</option>
<option value='Unrated' <?php if($rating == 'Unrated') echo 'selected="selected"'; ?> >Unrated</option>
</select>
<span class="error">* <?php echo $ratingErr;?></span>
<br><br>

<?php
echo "Genre: ";
if (isset($_GET['genre']))
{
	foreach ($_GET['genre'] as $selectedGenres)
		$selected[$selectedGenres] = "checked";
}
?>

<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Action'] ?> VALUE="Action">Action
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Adult'] ?> VALUE="Adult" >Adult
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Adventure'] ?> VALUE="Adventure" >Adventure
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Animation'] ?> VALUE="Animation" >Animation
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Comedy'] ?> VALUE="Comedy" >Comedy
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Crime'] ?> VALUE="Crime" >Crime
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Documentary'] ?> VALUE="Documentary" >Documentary
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Drama'] ?> VALUE="Drama" >Drama
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Family'] ?> VALUE="Family" >Family
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Fantasy'] ?> VALUE="Fantasy" >Fantasy <br>
<?php echo "&nbsp;&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;"; ?>
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Horror'] ?> VALUE="Horror" >Horror
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Musical'] ?> VALUE="Musical" >Musical
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Mystery'] ?> VALUE="Mystery" >Mystery
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Romance'] ?> VALUE="Romance" >Romance
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Sci-Fi'] ?> VALUE="Sci-Fi" >Sci-Fi
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Short'] ?> VALUE="Short" >Short
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Thriller'] ?> VALUE="Thriller" >Thriller
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['TV Show'] ?> VALUE="TV Show" >TV Show
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['War'] ?> VALUE="War" >War
<INPUT TYPE="checkbox" NAME="genre[]" <?php echo $selected['Western'] ?> VALUE="Western" >Western
<span class="error">* <?php echo $genreErr;?></span>
<br><br>

<input type="submit" name="submit" value="Add">
</form>

<?php
if (!empty($title) && !empty($company) && !empty($year) && !empty($rating) && !empty($selectedNum))
{
echo "<h2>You added the following Movie:</h2>";
echo "Title: $title<br>";
echo "Company: $company<br>";
echo "Year: $year<br>";
echo "Rating: $rating<br>";
if ($selectedNum > 0)
{
	echo "Genre: ";
	for($i=0; $i < $num; $i++)
	{
	 	echo $selectedNum[$i];
	 	if ($i < $num - 1)
	 		echo ", ";
	}
}
echo "<br>";
}
else
	echo "No film added.<br>";
?>


</div>
<?php include("./Footer.php"); ?>
</body>
</html>