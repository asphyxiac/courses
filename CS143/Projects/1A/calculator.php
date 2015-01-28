<html>
<body>
<title><?php echo "Calculator";?></title>
<link rel="shortcut icon" href="http://tinyurl.com/kwus8n7" type="image/x-icon" />
<h1>Simple PHP Calculator</h1>
<br>
<p>
<form method="GET">
<input type="text" name="expression"><input type="submit" value="Calculate!">
</p>
<b><dt>Accepted input:</dt></b>
<br>
<dd>Signed integers and real numbers.
<dd>Real numbers must begin with at least one integer.
<dd>The following operators, which follow standard operator precedence: +, -, *, and /.
<br><br>
<b><dt>Result: </b>
</body>
</html>

<?php
$pattern = "#^(?>[+\-]?(\d+\.?\d*))(\s+)?([+\-*/]+(\s+)?[\-|\+]?(\d+\.?\d*))*$#x";
$raw = $_GET["expression"]; // Kept for printing purposes at the end.
$string = str_replace(" ", "", $raw);

// Check for repeated operators.
if (preg_match("/[\+\-\/\*]{2,}/", $string))
{
	// This makes 1--1 work
	if (preg_match("/-{2,2}/", $string) && !preg_match("/-{3,}/", $string))
	{
		$string = str_replace("--", "- -", $string);
	}
	// This catches ++
	elseif (preg_match("/\+{2,2}/", $string))
	{
		echo "++ is not a valid operator. ";
		// To accept '++' as valid input, uncomment line below:
		// $string = str_replace("++", "+ +", $string);
	}
}

// Check for pattern match AND (2 or more +/-) 
// AND (// or ** or +/- followed by / or *).
if (preg_match($pattern, $string) && !preg_match("/\+{2,}|\-{2,}/", $string) && !preg_match("%(/|\*|-|\+)(\*|/)+%", $string))
{
	preg_match($pattern, $string, $matches);

	function e($errno, $errstr, $errfile, $errline)	{	}
	set_error_handler('e');

	eval("\$result =".$matches[0].";");

	// Eval returns "false" if forced to divide by zero.
	// Do not display result in this case.
	if ($result !== false)
	{
		echo $raw." = ".$result;
	}
	else
		echo "Division by zero.";
}
else if( empty($raw) )
{
	echo "Please enter an expression to be evaluated.";
}
else
{
	echo "$raw is not a valid input.";
}
?>

