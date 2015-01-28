<html>
<body>
<title><?php echo "Query";?></title>
<link rel="shortcut icon" href="http://tinyurl.com/nvnz328" type="image/x-icon" />
<h2>Please type your SQL query in the box below:</h2>
<p>
<form method="GET">
<TEXTAREA NAME="query" ROWS=8 COLS=60>
<?php echo htmlspecialchars($_GET['query']);

// NOTE: information on how to do make text persistent in the
// text area box was used from this stackoverflow reposnse:
// http://stackoverflow.com/questions/2296347/how-can-i-keep-a-value-in-a-text-input-after-a-submit-occurs

?>
</TEXTAREA>
<input type="submit" value="Submit">
</form>
</p>

<br><br>
<b><dt>Result from MySQL: <br></b>
</body>
</html>

<?php

// http://oak.cs.ucla.edu/cs143/project/php/php_db.html

// Given a query, do the following:
if ($_GET["query"])
{
	// Store query.
	$new_query = $_GET["query"];
	// Establish connection using username: cs143 and no password
	$db_connection = mysql_connect("localhost", "cs143", "");

	// Select database to query.
	mysql_select_db("CS143", $db_connection);

	// Get query, check if successful or end.
	$output = mysql_query($new_query, $db_connection);
	if (!output)
		die("Query could not be performed.<br>" . mysql_error());

	// Run query, display attributes.
	echo '<table border="1">';
	echo "<tr>";
	while ($fieldCount < mysql_num_fields($output))
	{
		$field = mysql_fetch_field($output, $fieldCount);
		echo "<th>$field->name</th>";
		$fieldCount++;
	}
	echo "</tr>";

	// Process row by row.
	while ($attr = mysql_fetch_row($output))
	{
		echo "<tr>";
		for ($attrCount = 0; $attrCount < $fieldCount; $attrCount++)
		{
			echo '<td align="center">';

			// If attr is not NULL, display it
			// (using ternary logic to avoid verbosity).
			$attribute = $attr[$attrCount] ? $attr[$attrCount] : 'N/A';
			echo "$attribute";
			echo '</td>';
		}
		echo "</tr>";
	}
	echo "</table>";

	// Close connetion!
	mysql_close($db_connection);
}
?>

