//
//  exam.cpp
//  Recursion
//
//  Created by Prianna Ahsan on 2/25/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>

const int INT_MIN = 0;
struct Node
{
    int data;
    Node* child[4];
};

int treeMax( Node* p)
{
    int max = INT_MIN;
    
    if(p)
    {
        max = (*p).data;
        
        if( p->child[0] )
            max = max > treeMax(p->child[0]) ? max : treeMax(p->child[0]);
        
        if( p->child[1] )
            max = max > treeMax(p->child[1]) ? max : treeMax(p->child[1]);
        
        if( p->child[2] )
            max = max > treeMax(p->child[2]) ? max : treeMax(p->child[2]);
        
        if( p->child[3] )
            max = max > treeMax(p->child[3]) ? max : treeMax(p->child[3]);
    }
    
    return max;
}