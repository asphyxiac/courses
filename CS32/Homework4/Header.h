

#ifndef Homework4_Header_h
#define Homework4_Header_h

struct Node
{
    // Constructor for Node: prevent creation of empty nodes.
    Node( int value )
    :m_value(value), left(NULL), right(NULL), parent(NULL)
    {}
    
    
    int m_value; // Each node must hold a value.
    Node* left; // Points to left sub-tree, or NULL.
    Node* right; // Points to right sub-tree, or NULL.
    Node* parent; // Points to the parent node or NULL if this node is root.
};

#endif
