#include "StudentMultiset.h"
#include <iostream>
#include <cassert>

using namespace std;

int main()
{
    StudentMultiset sms;
    sms.add(10001003);
    sms.add(10001003);
    sms.add(10001003);
    assert(sms.add(10001003));
    assert(sms.size()== 4);
    sms.add(70403043);
    sms.add(43303493);
    sms.add(10000133);
    sms.print();
    cout << endl;
    StudentMultiset sms2;
    sms2 = sms;
    sms2.add(39323933);
    sms2.print();
    cout << endl;
    assert(sms2.size() == 8);
    assert(sms.size() == 7);
    StudentMultiset sms3 = sms2;
    StudentMultiset sms4(sms3);
    sms3.print();
    cout << endl;
    sms4.print();
    
}
