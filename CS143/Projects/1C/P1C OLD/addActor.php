<!DOCTYPE html>
<html>
<head>
<title>Add Actor/Director</title>
<link href="./Site.css" rel="stylesheet">
<style>
.error {color: #FF0000;}
</style>
</head>

<?php include("./Header.php"); ?>

<div id="main">
<h1>Update the Actor/Director Database</h1>
<h2>Add new actor or director:</h2>

<?php
// error fields
$personErr = $firstNameErr = $genderErr = $dobErr = $dodErr = "";
$person = $firstName = $gender = $dob = $dod = "";

if ($_SERVER["REQUEST_METHOD"] == "GET")
{

// collect first name and check if it is legit
	if (empty($_GET["firstName"]))
		$firstNameErr = "Name is required";
	else
	{
		$firstName = test_input($_GET["firstName"]);
    	 // check if name only contains letters and whitespace
     	if (!preg_match("/^[a-zA-Z ]*$/",$name))
       		$firstNameErr = "Only letters and white space allowed.";
	}

// repeat for last name
	$lastName = test_input($_GET["lastName"]);
 	 // check if name only contains letters and whitespace
   	if (!preg_match("/^[a-zA-Z ]*$/",$lastName))
   		$firstNameErr = "Only letters and white space allowed.";

// store gender
	if (empty($_GET["gender"]))
		$genderErr = "Gender is required.";
	else
		$gender = test_input($_GET["gender"]);

// get DOB
	if ((empty($_GET["dobYear"])) OR (empty($_GET["dobMonth"])) OR (empty($_GET["dobDay"])))
		$dobErr = "Date of Birth is required.";
	else
	{
		$dobMonth = $_GET["dobMonth"];
		$dobDay = $_GET["dobDay"];
		$dobYear = $_GET["dobYear"];

		$dob = $dobMonth.'-'.$dobDay.'-'.$dobYear;
		//$date = $dob;
	}

// store DOD
	$dodMonth = $_GET["dodMonth"];
	$dodDay = $_GET["dodDay"];
	$dodYear = $_GET["dodYear"];

	$dod = $dodMonth.'-'.$dodDay.'-'.$dodYear;
	//$dod = (int)$_GET["dodMonth"].'-'.(int)$_GET["dodDay"].'-'.(int)$_GET["dodYear"];

// store type of person - actor or director
	if (empty($_GET["person"]))
		$personErr = "Person type is required.";
	else
		$person = test_input($_GET["person"]);
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
First Name: <input type="text" name="firstName" value="<?php echo $firstName;?>">
			<span class="error">* <?php echo $firstNameErr;?></span> <br><br>
Last Name: <input type="text" name="lastName" value="<?php echo $lastName;?>"> <br><br>

<INPUT TYPE="radio" name="gender"
<?php if (isset($gender) && $gender=="Male") echo "checked";?>  value="Male">Male
<INPUT TYPE="radio" name="gender"
<?php if (isset($gender) && $gender=="Female") echo "checked";?>  value="Female">Female
<span class="error">* <?php echo $genderErr;?></span>
<br><br>


<?php	echo "Date of Birth: "; ?>
<td><select name ='dobMonth' value='$dobMonth'>
<option value=''></option>
<option value='1' <?php if($dobMonth == '1') echo 'selected="selected"'; ?> >Jan</option>
<option value='2' <?php if($dobMonth == '2') echo 'selected="selected"'; ?> >Feb</option>
<option value='3' <?php if($dobMonth == '3') echo 'selected="selected"'; ?> >Mar</option>
<option value='4' <?php if($dobMonth == '4') echo 'selected="selected"'; ?> >Apr</option>
<option value='5' <?php if($dobMonth == '5') echo 'selected="selected"'; ?> >May</option>
<option value='6' <?php if($dobMonth == '6') echo 'selected="selected"'; ?> >June</option>
<option value='7' <?php if($dobMonth == '7') echo 'selected="selected"'; ?> >July</option>
<option value='8' <?php if($dobMonth == '8') echo 'selected="selected"'; ?> >Aug</option>
<option value='9' <?php if($dobMonth == '9') echo 'selected="selected"'; ?> >Sept</option>
<option value='10' <?php if($dobMonth == '10') echo 'selected="selected"'; ?> >Oct</option>
<option value='11' <?php if($dobMonth == '11') echo 'selected="selected"'; ?> >Nov</option>
<option value='12' <?php if($dobMonth == '12') echo 'selected="selected"'; ?> >Dec</option>
</select>
<?php echo '<select name="dobDay">'; echo '<option value=""></option>';
	  for($i = 1; $i <= 31; $i++)
	  {
		$i = str_pad($i, 2, 0, STR_PAD_LEFT);
		{
			echo "<option value='$i'>$i";
			if($dobDay == $i)
			{
				$sel = 'selected="selected"';
				echo '<option '.$sel.' value ="'.$i.'">'.$i.'</option>';
			}
			echo '</option>';
		}
	  }
echo '</select> ';
echo '<select name="dobYear">'; echo '<option value=""></option>';
	for($i = date('Y'); $i >= date('Y', strtotime('-100 years')); $i--)
	{
		echo "<option value='$i'>$i";
		if ($dobYear == $i)
		{
			$sel = 'selected="selected"';
			echo '<option '.$sel.' value = "'.$i.'">'.$i.'</option>';
		}
		echo "</option>";
	}
echo '</select> ';
?>
<span class="error">* <?php echo $dobErr;?></span>
<br><br>

<?php
echo "Date of Death: "; ?>
<td><select name ='dodMonth' value='$dodMonth'>
<option value=''></option>
<option value='1' <?php if($dodMonth == '01') echo 'selected="selected"'; ?> >Jan</option>
<option value='2' <?php if($dodMonth == '02') echo 'selected="selected"'; ?> >Feb</option>
<option value='3' <?php if($dodMonth == '03') echo 'selected="selected"'; ?> >Mar</option>
<option value='4' <?php if($dodMonth == '04') echo 'selected="selected"'; ?> >Apr</option>
<option value='5' <?php if($dodMonth == '05') echo 'selected="selected"'; ?> >May</option>
<option value='6' <?php if($dodMonth == '06') echo 'selected="selected"'; ?> >June</option>
<option value='7' <?php if($dodMonth == '07') echo 'selected="selected"'; ?> >July</option>
<option value='8' <?php if($dodMonth == '08') echo 'selected="selected"'; ?> >Aug</option>
<option value='9' <?php if($dodMonth == '09') echo 'selected="selected"'; ?> >Sept</option>
<option value='10' <?php if($dodMonth == '10') echo 'selected="selected"'; ?> >Oct</option>
<option value='11' <?php if($dodMonth == '11') echo 'selected="selected"'; ?> >Nov</option>
<option value='12' <?php if($dodMonth == '12') echo 'selected="selected"'; ?> >Dec</option>
</select>
<?php echo '<select name="dodDay">'; echo '<option value=""></option>';
	  for($i = 1; $i <= 31; $i++)
	  {
		$i = str_pad($i, 2, 0, STR_PAD_LEFT);

		echo "<option value='$i'>$i";
		if($dodDay == $i)
		{
			$sel = 'selected="selected"';
			echo '<option '.$sel.' value ="'.$i.'">'.$i.'</option>';
		}
		echo '</option>';
	  }
echo '</select> ';
echo '<select name="dodYear">'; echo '<option value=""></option>';
	  for($i = date('Y'); $i >= date('Y', strtotime('-100 years')); $i--)
	  {
		echo "<option value='$i'>$i";
		if ($dodYear == $i)
		{
			$sel = 'selected="selected"';
			echo '<option '.$sel.' value = "'.$i.'">'.$i.'</option>';
		}
		echo "</option>";
	  }
echo '</select> ';
echo 'Leave blank if still living.';
?> <br><br>

<INPUT TYPE="radio" name="person"
<?php if (isset($person) && $person=="Actor") 	echo "checked";?> value="Actor">Actor
<INPUT TYPE="radio" name="person"
<?php if (isset($person) && $person=="Director") echo "checked";?> value="Director">Director
<span class="error">* <?php echo $personErr;?></span>
<br><br>

<input type="submit" name="submit" value="Add">
</form>

<?php
if (!empty($firstName) && !empty($gender) && !empty($dob) && !empty($person))
{
	echo "<h2>You added a $person with the following data:</h2>";
	echo "Name: ".$firstName." ". $lastName."<br>";
	echo "Gender: $gender<br>";
	echo "DOB: $dob<br>";
	echo "DOD: ";
	if (empty($dod) OR ($dod=='0-0-0'))
		echo "Living.<br>";
	else
 		echo "$dod<br>";
	echo "Profession: $person<br>";
}
else
	echo "Nothing added.<br>";
?>


</div>
<?php include("./Footer.php"); ?>
</body>
</html>