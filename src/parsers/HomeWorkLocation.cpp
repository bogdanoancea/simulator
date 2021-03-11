/*
 * HomeWorkLocation.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#include <parsers/HomeWorkLocation.h>

HomeWorkLocation::HomeWorkLocation(double x, double y, double z, double sdx, double sdy, double sdz) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_sdx = sdx;
	m_sdy = sdy;
	m_sdz = sdz;
}

HomeWorkLocation::~HomeWorkLocation() {
	// TODO Auto-generated destructor stub
}

