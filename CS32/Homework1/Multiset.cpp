#include <iostream>
#include "Multiset.h"

using namespace std;

Multiset::Multiset()
: m_currentsize(0), m_multiset(), m_contained(0), m_newMax(DEFAULT_MAX_ITEMS)
{
}

bool Multiset::empty() const
{
    for (int i = 0; i < m_newMax; i++) 
    {
        if (m_multiset[i].count > 0)
        {
            return false;
        }
    }
    
    return true;
}

int Multiset::size() const
{
    int tempSize = 0;
    
    if (empty()) 
    {
        return tempSize;
    }
    
    else 
    {
        for (int i = 0; i < m_newMax; i++) 
        {
            if (m_multiset[i].count > 0) 
            {
                tempSize += m_multiset[i].count;
            }
        }
        
        return tempSize;
    }
}

int Multiset::uniqueSize() const
{
    int uniqueSize = 0;
    
    if (empty()) 
    {
        return uniqueSize;
    }
    
    for (int i = 0; i < m_newMax; i++) 
    {
        if (m_multiset[i].count > 0) 
        {
            uniqueSize++;
        }
        
    }
    
    return uniqueSize;
}

bool Multiset::insert(const ItemType &value)
{
    if (contains(value)) 
    {
        m_multiset[m_contained].count++;
        return true;
    }
    
    else
    {
        if( m_currentsize == m_newMax ) // Checking for full multiset.
            return false;
       
        int p = 0;
        
        if (m_multiset[m_currentsize].count < 1) 
        {
            m_multiset[m_currentsize].entry = value;
            m_multiset[m_currentsize].count = 1;
            m_currentsize = uniqueSize();
            return true;
        }
            
        else
        {
            while(m_multiset[p].count > 0 && p < m_newMax)
            {
                p++;
            }
            
            m_multiset[p].entry = value;
            m_multiset[p].count = 1;
            m_currentsize = uniqueSize();
            return true;
        }
            
        
    }
}

int Multiset::erase(const ItemType &value)
{
    int temp, tempCount;
    if (contains(value))
    {
        temp = 1;
        m_multiset[m_contained].count--;
        tempCount = (m_multiset[m_contained].count);
        if (tempCount == 0) 
        {
            m_currentsize = uniqueSize();
        }
        return temp;
    }
    
    else
        temp = 0;
        return temp;
}

int Multiset::eraseAll(const ItemType &value)
{
    int temp = 0, tempCount;
    
    if (contains(value))
    {
        tempCount = m_multiset[m_contained].count;
        m_multiset[m_contained].count = 0;
        m_currentsize = uniqueSize();
        return tempCount;
    }
    
    else
        return temp;
}

bool Multiset::contains(const ItemType &value)
{
    if (empty()) 
    {
        return false;
    }
    
    for (int i = 0; i < m_newMax; i++) 
    {
        if (m_multiset[i].entry == value && m_multiset[i].count > 0) 
        {
            setContained(i);
            return true;
        }
    }
    
    return false;
}

int Multiset::count(const ItemType &value)
{
    int temp = 0;
    
    if (contains(value))
    {
        temp = m_multiset[m_contained].count;
        return temp;
    }
    
    else
        return temp;
}

int Multiset::get(int i, ItemType &value) const
{
    if (empty()) 
    {
        return 0;
    }
    
    else if (i >= 0 && i < uniqueSize()) 
    {
        value = m_multiset[i].entry;
        return m_multiset[i].count;
    }
    
    else
        return 0;
}

void Multiset::swap(Multiset &other)
{
    
    if (this == &other) 
    {
        return;
    }
    
    Multiset temp = other;
    
    other = *this;
    
    *this = temp;
}

void Multiset::setContained(int i)
{
    m_contained = i;
}

/*
 void Multiset::dump() const
{
    for (int i = 0; i < m_newMax; i++) 
    {
        cout << m_multiset[i].count << " " << m_multiset[i].entry << endl;
        cout << m_currentsize << " " << uniqueSize() << endl;
    }
}
*/