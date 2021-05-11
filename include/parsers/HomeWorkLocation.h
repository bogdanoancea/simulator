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
 * HomeWorkLocation.h
 *
 *  Created on: Mar 11, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_HOMEWORKLOCATION_H_
#define INCLUDE_HOMEWORKLOCATION_H_

#include <string>

using namespace std;

class HomeWorkLocation {
public:
	HomeWorkLocation(double x, double y, double z, double sdx, double sdy, double sdz);
	HomeWorkLocation() = delete;
	virtual ~HomeWorkLocation();
	string toString();
	double m_x, m_y, m_z;
	double m_sdx, m_sdy, m_sdz;
};

#endif /* INCLUDE_HOMEWORKLOCATION_H_ */
