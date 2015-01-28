/** @file EnergySource.h */

#ifndef ENERGYSOURCE_H
#define ENERGYSOURCE_H

class EnergySource
{
  public:
        // Constructor
    EnergySource(int r, int c);

        // Accessors
    int row() const;
    int col() const;

  private:
    int m_row;
    int m_col;
};

#endif
