/****************************************************
* Author: Prianna Ahsan							    *
*		  prianna@ucla.edu						    *	
* About: This is a simple program that tests the    *
* Collatz conjecture for integers less than 1000000 *
* and greater than 0.								*
****************************************************/
#include <iostream>
#include <cmath>
using namespace std;

int cycleCalc( int x ) //Function that applies 3n+1 to odd integers.
{
	int cycles = 1;
	while( x != 1 ) //Testing to see if cycle is complete.
	{
		if( x%2 == 0 ) //Testing for oddness.
		{
			x = x/2;
			//cout << x << endl;
		}
	
		else
		{
			x = x*3 + 1;
			//cout << x << endl;
		}
		
		cycles++;
		
		//cout << "Cycles: " << cycles << endl;
		
	}
	
	
	return cycles;
}

int main()
{
	int x;
	
	cout << "Start: ";
	cin >> x;
	
	while( x > 0 ) //Testing for negativity.
	{
		int y, counter = 0, max = 0;
		cout << "End: ";
		cin >> y;
		
		for( int i = x; i < y; i++ )
		{
			counter = cycleCalc( i );
			
			if( counter >= max )
				max = counter;
			
		}
		
		
		cout << "Max: " << max << "\n" << endl;
		
		cout << "Start: ";
		cin >> x;
	}
	
	return 0;
}