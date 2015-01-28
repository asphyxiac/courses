/** @file EnergySource.cpp */

#include <iostream>
#include <iomanip>
#include "EnergySource.h"

using namespace std;

EnergySource::EnergySource(int r, int c)
 : m_row(r), m_col(c)
{
    if (r < 0  ||  c < 0)
    {
        cout << "***** EnergySource created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
}

int EnergySource::row() const
{
    return m_row;
}

int EnergySource::col() const
{
    return m_col;
}