#ifndef StudentMultiset_h
#define StudentMultiset_h

#include "Multiset.h"

class StudentMultiset
{
public:
    StudentMultiset();       // Create an empty student multiset.
    
    StudentMultiset(const StudentMultiset &other); // Copy constructor
    
    StudentMultiset& operator= (const StudentMultiset &other);
    
    bool add(unsigned long id);
    // Add a student id to the StudentMultiset.  Return true if and only
    // if the id was actually added.
    
    int size() const;
    // Return the number of items in the StudentMultiset.  If an id was
    // added n times, it contributes n to the size.
    
    void print() const;
    // Print every student id in the StudentMultiset one per line; print
    // as many lines for each id as it occurs in the StudentMultiset.
    
private:
     
    Multiset m_database;
};


#endif