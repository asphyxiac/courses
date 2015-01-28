/** @file Battery.h */

#ifndef BATTERY_H
#define BATTERY_H

class Battery
	{
	  public:
	    Battery(int initialLevel);
	    int level() const;
	    void decrease();

	  private:
		int m_level;

	};

#endif