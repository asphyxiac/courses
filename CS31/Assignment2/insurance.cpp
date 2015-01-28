/*********************************************
Author: Prianna Ahsan, prianna@ucla.edu
ID: 704068040
Course: CS31, Section 1D
Description: A simple program that accepts 
user input and simulates an insurance payment 
calculator.
*********************************************/
#include <iostream>
//#include <cmath>
using namespace std;


//A function that rounds up the number of miles.
/*int milesFn( float miles )
{
	int newMiles = ceil( miles );
	int calcMiles = (newMiles/100);
	int milesRemainder = (newMiles%100);
	
	if ( milesRemainder >= 50 )
		calcMiles = calcMiles + 1;
		
	return calcMiles;
}*/

int main()
{
	int age, tickets;
	float miles;
	char student;
	const int AGE_MIN = 42;
	const int AGE_MAX = 71;
	const int AGE_STU = 35; //Max age of students.
	const float RATESTD = 602.50; //Standard rate.
	const float RATEEXP = 400.00;
	const float MILESTD = 0.152;
	const float MILEGD = 0.1235;
	const float STUDISC = 0.05; //Student discount rate.
	float calcRateSD, calcRateGD = 0, calcRate;
	
	cout.setf( ios::fixed );
    cout.setf( ios::showpoint );
    cout.precision( 2 ); 
    
	cout << "Age of driver: ";
	cin >> age;
	
	cout << "Good student? (y/n): ";
	cin >> student;
	
	cout << "Number of tickets: ";
	cin >> tickets;
	
	cout << "Miles per week driven: ";
	cin >> miles;
	
	cout << "---\nThe rate is $";
	

	
	//else
	//{
		if( age > AGE_MIN && age < AGE_MAX && tickets <= 1 ) //Checking for age-related discount.
		
			calcRateSD = RATEEXP;
			
		else if( student == 'y' && age < AGE_STU ) //Checking for student status.
		{
			//int calcMiles = milesFn( miles );
			//float stuRate = calcMiles*MILESTD + RATESTD;
			float stuRate = miles*MILESTD + RATESTD;
			float stuDisc = STUDISC*stuRate;
			calcRateSD = stuRate - stuDisc;
		}
		
		else
		{
			//int calcMiles = milesFn( miles );
			//calcRate = calcMiles*MILESTD + RATESTD;
			calcRateSD = miles*MILESTD + RATESTD;
		}
	
		if( tickets == 0 )
			//int calcMiles = milesFn( miles );
			//calcRate = calcMiles*MILEGD + RATESTD;
			calcRateGD = miles*MILEGD + RATESTD; 
		
	//}
	
	if ( calcRateGD > 0 && calcRateSD < calcRateGD )
	{
		calcRate = calcRateSD;
		//cout << "Student status: " << student << endl;
		//cout << "Printing student rate." << endl;
	}	
	else if ( calcRateGD > 0 && calcRateGD < calcRateSD )
	{
		calcRate = calcRateGD;
		//cout << "Student status: " << student << endl;
		//cout << "Printing good driver rate." << endl;
		
	}	
	else
	{
		calcRate = calcRateSD;
		//cout << "Student status: " << student << endl;
		//cout << "Printing std rate." << endl;
	}
	
	cout << calcRate << endl;
	
	return 0;
}