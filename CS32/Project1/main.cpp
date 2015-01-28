/** @file main.cpp */

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Robot.h"
#include "globals.h"
#include "Valley.h"
#include "EnergySource.h"
using namespace std;

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

// You can use whatever main routine you want.  In fact, try different
// things that will thoroughly test your classes.  This main routine is
// the one that the sample executable uses.


int main()
{
        // Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));

        // Create a 10x10 valley
    Valley v(10, 10);

        // Populate it with three robots
    v.addRobot("Abner", 0, 0, SOUTH);
    v.addRobot("Betty", 9, 9, NORTH);
    v.addRobot("Chris", 0, 9, SOUTH);
    v.addRobot("David", 3, 3, WEST);
    
      // Add energy sources at (1,1), (1,8), (8,1), (8,8)
    for (int r = 1; r < v.rows(); r += 7)
        for (int c = 1; c < v.cols(); c += 7)
            v.addEnergySource(r, c);

        // Step until all robots are dead, displaying the valley each time
    do
    {
        v.display();
        cout << endl;
        cout << "Press Enter to continue ";
        cin.ignore(10000, '\n');
    } while(v.step());

    cout << "All robots are dead" << endl;
}

