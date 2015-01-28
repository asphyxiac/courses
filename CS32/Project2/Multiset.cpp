// @file: Multiset.cpp
#include <iostream>
#include "Multiset.h"

using namespace std;

void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    Multiset* copy = new Multiset( ms1 );
    
    for (int i = 0; i < ms2.uniqueSize(); i++) 
    {
        ItemType value;
        int x = ms2.get( i, value );
        
        for (int j = 0; j < x; j++) 
        {
            copy->insert(value);
        }
    }
    
    result = *copy;
    delete copy;
}

void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result)
{
    Multiset* copy = new Multiset( ms1 );
    
    for (int i = 0; i < ms1.uniqueSize(); i++) 
    {
        ItemType value;
        int x = ms1.get( i, value );
        
        if ( ms2.contains(value) && (x >= ms2.count(value)) )
        {
            x = ms2.count(value);
            for (int j = 0; j < x; j++) 
            {
                copy->erase(value);
            }
        }
    }
    
    result = *copy;
    delete copy;
}


Multiset::Multiset()
: m_head(NULL), m_tail(NULL), m_size(0), m_uniqueSize(0)
{
    
}

Multiset::Multiset( const Multiset& other )
: m_head(NULL), m_tail(NULL), m_size(0), m_uniqueSize(0)
{
    if ( (other.m_head && other.m_tail) ) // If other multiset has nodes.
    {
        int tempSize = other.m_uniqueSize;
        
        // To preserve order, we pull values tail first. Insert creates new
        // nodes head first. This is unnecessary (order preservation) but
        // I wanted to create exact copies.
        for (int i = tempSize-1; i >= 0; i--)
        {
            ItemType value;
            int x = other.get(i, value);
            
            for (int j = 0; j < x; j++) 
            {
                insert(value);
            }
        }
    }
    
}

Multiset& Multiset::operator=(const Multiset &other )
{
    if (this != &other) 
    {
        Multiset* temp = new Multiset ( other );
        NodePtr temp_head = m_head;
        
        this->m_head = temp->m_head;
        this->m_tail = temp->m_tail;
        this->m_size = temp->m_size;
        this->m_uniqueSize = temp->m_uniqueSize;
        
        temp->m_head = temp_head;
        
        delete temp;
    }
    
    return *this;
}

Multiset::~Multiset()
{
    NodePtr discard = m_head;
    
    while( discard )
    {
        NodePtr temp = discard->getNext();
        delete discard;
        discard = temp;
    }
    
}



// Define insert, get, count, swap

bool Multiset::insert( const ItemType& value )
{
    NodePtr newest = find(value);
    
    if ( newest )
        newest->setCount( (newest->getCount()) + 1 );

    else
    {
        // Inserting from top, so we set our new node's next pointer to point
        // to the node currently pointed to by m_head.
        newest = new Node( value, 1, m_head, NULL );
        
        if (m_head) // Checking for empty list.
            m_head->setPrev(newest); // Set previous node's pointer to newest.
        else
            m_tail = newest; // m_head = m_tail iff inserting into empty list.

        m_head = newest; // Linking head to newest node.
        m_uniqueSize += 1;
    }
    
    m_size += 1;
    return true;
}

int Multiset::count( const ItemType& value ) const
{
    NodePtr count = find(value);
    
    if (count) 
        return count->getCount();
    else
        return 0;
}

int Multiset::get(int i, ItemType& value) const
{
    if (i < 0  ||  i >= m_uniqueSize)
        return 0;
    
    NodePtr next = m_head;
    
    for (int j = 0; j < i; j++)
    {
        if (next)
            next = next->getNext(); // Let's make sure next exists before we try
                                    // to pull values out of it!
    }
    
    value = next->getVal();
    return next->getCount();
}

void Multiset::swap(Multiset& other)
{
    if (this != &other) 
    {
        NodePtr temp = other.m_head;
        
        other.m_head = this->m_head;
        this->m_head = temp;
        
        temp = other.m_tail;
        
        other.m_tail = this->m_tail;
        this->m_tail = temp;
        
        int tempSize = other.m_size;
        
        other.m_size = this->m_size;
        this->m_size = tempSize;
        
        tempSize = other.m_uniqueSize;
        
        other.m_uniqueSize = this->m_uniqueSize;
        this->m_uniqueSize = tempSize;
    }
}

int Multiset::doErase( const ItemType& value, bool all )
{
    NodePtr temp = find(value);
    
    if (temp) 
    {
        NodePtr before = temp->getPrev();
        NodePtr after = temp->getNext();
        if (!all && temp->getCount() > 1) 
        {
            int count = temp->getCount();
            temp->setCount(count-1);
            m_size--;
            return 1;
        }
        
        if (before) // Check if there's anything in front of node.
            before->setNext(temp->getNext()); // Linking pointers.
        else
            m_head = after; // If node to be deleted is in front
                            // set m_head to node after deleted node.
        
        if (after) // Check if there's anything behind node.
            after->setPrev(temp->getPrev()); // Linking pointers.
        else
            m_tail = before; // If node to be deleted is at the tail
                             // set m_tail to node before deleted node.
        
        int erased = temp->getCount();
        delete temp;
        m_size -= erased;
        m_uniqueSize--;
        return erased;
    }
    
    else
        return 0;
}

Multiset::NodePtr Multiset::find( const ItemType& value ) const
{
    if (!m_head) 
        return NULL;
    
    for (NodePtr temp = m_head; temp!= NULL; temp = temp->getNext()) 
    {
        if (temp->getVal() == value)
            return temp;
    }
    
    return NULL;
}

void Multiset::dump()
{
    cerr << "Printing contents." << endl;
    
    NodePtr temp = m_head;
    
    while (temp) 
    {
        cerr << "Count: " << temp->getCount() << " Value: " << temp->getVal() << endl;
        temp = temp->getNext();
    }
    
    // Commented out code below checks to make sure that get function works
    // properly.
    /*
    ItemType value;
    int tempSize = this->uniqueSize();
    int x;

    for (int k = 0; k < tempSize; k++) 
    {
        x = get(k, value);
        
        cerr << "Count: " << x << " Value: " << value << endl;
    }*/
}



// Begin Node member definitions

Multiset::Node::Node()
: m_next(NULL), m_prev(NULL), m_count(0)
{
}

Multiset::Node::Node(ItemType value, int count, NodePtr next, NodePtr prev)
: m_value(value), m_count(count), m_next(next), m_prev(prev)
{
}

Multiset::Node::Node( const Node& original )
{
    m_value = original.m_value;
    m_count = original.m_count;
    m_next = original.m_next;
    m_prev = original.m_prev;
}

Multiset::Node& Multiset::Node::operator=( const Node& other )
{
    if (this != &other )
    {
        this->m_value = other.m_value;
        this->m_count = other.m_count;
        this->m_next = other.m_next;
        this->m_prev = other.m_prev;
    }
    
    return *this;
}
