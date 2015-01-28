/** @file Battery.cpp */

#include "Battery.h"
using namespace std;

Battery::Battery( int initialLevel )
 : m_level( initialLevel )
{
	// empty
}

int Battery::level() const
{
	return m_level;
}

void Battery::decrease()
{
	if( m_level > 0 )
		m_level--;
	else
		return;
}