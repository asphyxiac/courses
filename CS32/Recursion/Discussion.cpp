//
//  Discussion.cpp
//  Recursion
//
//  Created by Prianna Ahsan on 2/22/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct Movie;

struct Person
{
    string name;
};

struct Movie
{
    string title; 
};

map< Person, vector<Movie> > fav;

bool isFav( Person& p, Movie& m )
{
    map< Person, vector<Movie> >::iterator movIt;
    
    if ( find( fav[p].begin(), fav[p].end(), m ) == fav[p].end() ) 
        return false;
    
    
}
