#include "StudentMultiset.h"
#include <iostream>

using namespace std;

StudentMultiset::StudentMultiset()
: m_database()
{
}

StudentMultiset::StudentMultiset(const StudentMultiset &other)
: m_database()
{
    m_database = other.m_database;
}

StudentMultiset& StudentMultiset::operator=(const StudentMultiset &other)
{
    if (this == &other) 
    {
        return *this;
    }
    
    m_database = other.m_database;
    return *this;
    
}

bool StudentMultiset::add(unsigned long ID)
{
    
    if(m_database.insert(ID))
    {
        return true;
    }
    
    else 
        return false;
}

int StudentMultiset::size() const
{
    return m_database.size();
}

void StudentMultiset::print() const
{
    for (int i = 0; i < m_database.uniqueSize(); i++) 
    {
        unsigned long x;
        int tempCount = m_database.get(i, x);
        for (int j = 0; j < tempCount; j++) {
            cout << x << endl;
        }
    }
}
