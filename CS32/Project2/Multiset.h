// @file: Multiset.h

#ifndef MULTISET_H
#define MULTISET_H

//#include <string>

//typedef std::string ItemType;
typedef unsigned long ItemType;

class Multiset
{
public:
    Multiset();             // Create an empty multiset.
    
    bool empty() const;  // Return true if the multiset is empty, otherwise false.
    
    int size() const;
    // Return the number of items in the multiset.  For example, the size
    // of a multiset containing "cumin", "cumin", "cumin", "turmeric" is 4.
    
    int uniqueSize() const;
    // Return the number of distinct items in the multiset.  For example,
    // the uniqueSize of a multiset containing "cumin", "cumin", "cumin",
    // "turmeric" is 2.
    
    bool insert(const ItemType& value);
    // Insert value into the multiset.  Return true if the value was
    // actually inserted.  Return false if the value was not inserted
    // (perhaps because the multiset has a fixed capacity and is full).
    
    int erase(const ItemType& value);
    // Remove one instance of value from the multiset if present.
    // Return the number of instances removed, which will be 1 or 0.
    
    int eraseAll(const ItemType& value);
    // Remove all instances of value from the multiset if present.
    // Return the number of instances removed.
    
    bool contains(const ItemType& value) const;
    // Return true if the value is in the multiset, otherwise false.
    
    int count(const ItemType& value) const;
    // Return the number of instances of value in the multiset.
    
    int get(int i, ItemType& value) const;
    // If 0 <= i < uniqueSize(), copy into value an item in the multiset
    // and return the number of instances of that item in the multiset.
    // Otherwise, leave value unchanged and return 0.
    
    void swap(Multiset& other);
    // Exchange the contents of this multiset with the other one.
    
    void dump();
    //Checking contents of multiset.
    
    // The big three.
    
    Multiset(const Multiset& original); // Copy constructor.
    
    ~Multiset(); // Destructor.
    
    Multiset& operator=(const Multiset& other); // Overloaded assignment operator.
    
private:
    
    class Node;
        
    typedef Node* NodePtr;

    NodePtr m_head;  // first item in the multiset
    NodePtr m_tail; // last item in the multiset
    int  m_uniqueSize;               // how many distinct items in the multiset
    int  m_size;                     // total number of items in the multiset
    
    // At any time, the elements of m_data indexed from 0 to m_uniqueSize-1
    // are in use.  m_size is the sum of the m_counts of those elements.
    
    NodePtr find(const ItemType& value) const;
    // Return pointer to the node whose m_value == value if there is
    // one, else -1
    
    int doErase(const ItemType& value, bool all);
    // Remove one or all instances of value from the multiset if present,
    // depending on the second parameter.  Return the number of instances
    // removed.
    
    // A private class to hold our nodes. We specify a default constructor
    // to make sure that node objects are initialized with NULL pointers.
    class Node
    {
    public:
        
        Node(); // Default constructor.
        Node(ItemType value, int count, NodePtr next, NodePtr prev);
        Node( const Node& original ); // Copy constructor.
        Node& operator=( const Node& other ); // Overloaded assignment operator.
        
        // Mutators
        void setNext( NodePtr next );
        void setPrev( NodePtr prev);
        void setCount( const int count );
        void setVal( const ItemType value );
        
        // Accessors
        NodePtr getNext() const;
        NodePtr getPrev() const;
        int getCount() const;
        ItemType getVal() const;
        
    private:
        
        ItemType m_value;
        int      m_count;
        NodePtr m_next;
        NodePtr m_prev;
    };
};

// Inline implementations

inline
int Multiset::size() const
{
    return m_size;
}

inline
int Multiset::uniqueSize() const
{
    return m_uniqueSize;
}

inline
bool Multiset::empty() const
{
    return size() == 0;
}

inline
int Multiset::erase(const ItemType& value)
{
    return doErase(value, false);
}

inline
int Multiset::eraseAll(const ItemType& value)
{
    return doErase(value, true);
}

inline
bool Multiset::contains(const ItemType& value) const
{
    return find(value);
}

inline
void Multiset::Node::setNext(Node* next)
{
    m_next = next;
}

inline
void Multiset::Node::setPrev(Node* prev)
{
    m_prev = prev;
}

inline
void Multiset::Node::setCount(const int count)
{
    m_count = count;
}

inline
void Multiset::Node::setVal(const ItemType value)
{
    m_value = value;
}

inline
Multiset::NodePtr Multiset::Node::getNext() const
{
    return m_next;
}

inline
Multiset::NodePtr Multiset::Node::getPrev() const
{
    return m_prev;
}

inline
ItemType Multiset::Node::getVal() const
{
    return m_value;
}

inline
int Multiset::Node::getCount() const
{
    return m_count;
}

// Combines values from two multisets and stores them in result.
void combine(const Multiset& ms1, const Multiset& ms2, Multiset& result);

// Subtracts value count from first multiset by value count of second multiset
// given that the multisets contain the same value and that value count of
// the second multiset doesn't exceed the value count of the first multiset
// (no negative value counts allowed in our multisets), and stores the values
// and differential counts in results.
void subtract(const Multiset& ms1, const Multiset& ms2, Multiset& result);


#endif // MULTISET_H