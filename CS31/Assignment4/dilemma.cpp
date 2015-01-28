/* 
Author: Prianna Ahsan (prianna@ucla.edu)
Class: CS 31
Sec: 1D
TA: E. Kostem
Project No.: 4

This program is simulates a prisoner's dilemma type game over 10000 iterations. 
See http://www.cs.ucla.edu/classes/summer12/cs31/Projects/4/spec.html for more 
details on the rewards/punishments for either cooperating or defecting. Please
see the file titled 'report.docx' for additional documentation and comments.
*/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

const int SIZE = 10;
const int ITERATIONS = 10000;

void assignInitialValues( string grid[][10], int size ); /* where each square in
                                                          the grid is randomly assigned the value "C" for cooperator or "D" for defector.*/

void iterateGrid( string grid[][10], int size ); /* where each square in the grid is 
                                                  modified according the rules provided above.*/

double percentageOfCooperation( string grid[][10], int size );

void printPercentage( double percentage ); /*{cout << "The percentage of cooperators is " 
                                            << percentage << "%\n";}*/

int main(){
	
	string grid[SIZE][SIZE];
	
	
	assignInitialValues( grid, SIZE );
	
	iterateGrid( grid, SIZE );
	
	double percentage = percentageOfCooperation( grid, SIZE );
	
	printPercentage( percentage );
	
}

void assignInitialValues( string grid[][10], int size ){
	
	srand((int) time(0)); //Truncating value returned by time(0).
	for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
			
			if(rand() % 2) //Randomly assigning 'C' or 'D'.
				grid[i][j] = "C";
			else
				grid[i][j] = "D";
    }
	
	
}

void iterateGrid( string grid[][10], int size ){
	
	int sentence[SIZE][SIZE];
	int counter = 0;
	const int NO_PLEA = 3, BOTH_PLEA = 5, RATTER = 2, RATTED = 10;
    
	while( counter < ITERATIONS )
	{
		for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
			{
				string cell[4];
				int neighbors = 0;
				sentence[i][j] = 0;
				
                
				if( i > 0 ){ //Checking top neighbor.
					cell[neighbors] = grid[i-1][j];
					neighbors++;
				}
				
				if( j > 0 ){ //Checking left neighbor.
					cell[neighbors] = grid[i][j-1];
					neighbors++;
				}
                
				if( i < 9 ){ //Checking bottom neighbor.
					cell[neighbors] = grid[i+1][j];
					neighbors++;
				}
				
				if( j < 9 ){ //Checking right neighbor.
					cell[neighbors] = grid[i][j+1];
					neighbors++;
                }
                
                
				for( int k = 0; k < neighbors; k++ )//Checking for prisoner status.
				{
					char status = cell[k][0];
					
					switch( status ){
						case 'C':
							if( grid[i][j] == "C" )//Calculating sentences.
								sentence[i][j] = sentence[i][j] + NO_PLEA;
							else
								sentence[i][j] = sentence[i][j] + RATTER;
							break;
						case 'D':
							if( grid[i][j] == "C" )
								sentence[i][j] = sentence[i][j] + RATTED;
							else
								sentence[i][j] = sentence[i][j] + BOTH_PLEA;
							break;
						default:
							break;
					}//endswitch		
				}//endfor
                
			}//endfor		
        }//endfor
        
		for (int i = 0; i < SIZE; i++){
	    	for (int j = 0; j < SIZE; j++){
				
				if( sentence[i][j] >= 20 && grid[i][j] == "D" ){
					grid[i][j] = "C";
				}
				
				else if( sentence[i][j] >= 20 && grid[i][j] == "C" ){
					grid[i][j] = "D";
				}
				
				else
					continue;
                
            }//endfor
		}//endfor
		
		counter++;
	}//endwhile
	
}

double percentageOfCooperation( string grid[][10], int size ){
	
	double percentage = 0.0;
	for (int i = 0; i < SIZE; i++){
    	for (int j = 0; j < SIZE; j++){
			if( grid[i][j] == "C" )
				percentage++;
		}
	}
	
	return percentage;
	
}

void printPercentage( double percentage ){
    
	cout << "The percentage of cooperators is " << percentage << "%\n";
}