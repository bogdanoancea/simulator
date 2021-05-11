/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * HomeWorkLocation.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <parsers/HomeWorkLocation.h>
#include <sstream>

using namespace std;

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

string HomeWorkLocation::toString() {
	ostringstream result;
	result << "X:" << m_x <<" Y:"<< m_y << " Z:" << m_z << " Sd_x:" << m_sdx << " Sd_y:" << m_sdy << " Sd_z:" << m_sdz;
	return (result.str());
}
