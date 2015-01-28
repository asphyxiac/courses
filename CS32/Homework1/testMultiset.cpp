#include "Multiset.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    Multiset ms;
    assert(ms.empty());
    unsigned long x = 999;
    assert(ms.get(0, x) == 0  &&  x == 999);  // x unchanged by get failure
    assert( ! ms.contains(42));
    ms.insert(42);
    ms.insert(42);
    assert(ms.size() == 2  &&  ms.uniqueSize() == 1);
    assert(ms.get(1, x) == 0  &&  x == 999);  // x unchanged by get failure
    assert(ms.get(0, x) == 2  &&  x == 42);
    ms.insert(66);
    ms.insert(66);
    unsigned long l1;
    int n1 = ms.get(1, l1);
    assert((l1 == 66  &&  n1 == 2)  ||  (l1 == 42  &&  n1 == 2));
    unsigned long l2;
    int n2 = ms.get(1, l2);
    assert(l2 == l1  &&  n2 == n1);
    assert(ms.count(42) == 2);
    ms.erase(42);
    assert(ms.count(42) == 1);
    ms.insert(66);
    assert(ms.count(66) == 3);
    assert(ms.uniqueSize() == 2);
    assert(ms.size() == 4);
    ms.eraseAll(66);
    assert(ms.count(66) == 0);
    Multiset ms2;
    ms.swap(ms2);
    int p = ms2.count(42);
    assert(p == 1);
    
    cout << "Passed all tests" << endl;
    
    //ms.dump();
}

/*
    Multiset ms1;
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("cumin");
    ms1.insert("turmeric");
    Multiset ms2;
    ms2.insert("coriander");
    ms2.insert("cumin");
    ms2.insert("cardamom");
    ms1.swap(ms2);  // exchange contents of ms1 and ms2
    
    int a = ms1.size(), b = ms1.count("coriander"), c = ms1.count("cumin"), d = ms1.count("cardamom"), e = ms1.count("Cardamom");
    
    assert(a == 3  &&  b == 1  && c == 1  &&  d == 1 && e == 0);
    
    assert(ms2.size() == 4  &&  ms2.count("cumin") == 3  &&
           ms2.count("turmeric") == 1);
    
    
    Multiset ms;
    ms.insert("cumin");
    assert(!ms.contains(""));
    ms.insert("nutmeg");
    ms.insert("");
    ms.insert("saffron");
    assert(ms.contains(""));
    ms.erase("cumin");
    assert(ms.size() == 3);
    assert(ms.contains("saffron")  &&  ms.contains("nutmeg")  &&
           ms.contains(""));
    
    cout << "passed" << endl;
}
*/