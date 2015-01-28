//
//  main.cpp
//  Recursion
//
//  Created by Prianna Ahsan on 2/21/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>

using namespace std;

unsigned long bitS( int n )
{
    if( n == 1 )
        return 2;
    if( n == 2 ) 
        return 4;
    if( n == 3 )
        return 7;
    
    unsigned long s = bitS(n-1)+bitS(n-2)+bitS(n-3);
    
    return s;
    
    
}

int main (int argc, const char * argv[])
{

    unsigned long n = bitS(4);
    cout << n << endl;
    return 0;
}

