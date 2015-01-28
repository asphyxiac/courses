				 Simple PHP Calculator version 1.0
					 	Released: 01/14/2014


AUTHORS
-------
Ahsan, Prianna
email: prianna@ucla.edu
SID: 704-068-040

Chemelekov, Rossen
email: rchemelekov@gmail.com
SID: 504-216-616

INCLUDED FILES
--------------
calculator.php
readme.txt (THIS)
	
GENERAL USAGE NOTES
-------------------
calculator.php consists of an HTML form that accepts user input (a mathematical expression) and a simple PHP script that receives raw form input via $_GET, validates the input with preg_match() and makes any necessary modifications before computing the result of the expression using eval(). The result is then printed to the page.

The calculator accepts both real and integer valued numbers. Real (floating point) numbers must be prefixed with at least one integer before the decimal.

The calculator accepts the following operations on floating point or integer values: +,-,/,*, which follow standard operator precedence.

The calculator treats any whitespace as non-significant. 

Examples of valid expressions are:
1--1 (Result: 2)
1.5*3-4 (Result: 0.5)
5- 4 (Result: 1)
0.4 - 0. 5 (Result: -0.1)
4*-7 (Result: -28)

Examples of invalid expressions:
1/0 (Division by zero)
1++1 (++ is not a valid operator)
1/*0 (not a valid input)

BUG REPORTING
-------------
Please send any bugs found to prianna@ucla.edu or rchemelekov@gmail.com