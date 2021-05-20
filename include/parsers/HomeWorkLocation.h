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

/**
 * This class encapsulates the information needed to generate the home or work location on the map. All
 * the information need to build a HomeWorkLocation object are read from the simulation configuration file.
 *
 *
 */
class HomeWorkLocation {
public:
	/**
	 * Constructor of the class. It builds a HomeWorkLocation object by setting its members using the values passed as parameters.
	 * @param x the x coordinate of a Home - Work location.
	 * @param y
	 * @param z
	 * @param sdx
	 * @param sdy
	 * @param sdz
	 */
	HomeWorkLocation(double x, double y, double z, double sdx, double sdy, double sdz);

	/**
	 *
	 */
	HomeWorkLocation() = delete;

	/**
	 *
	 */
	virtual ~HomeWorkLocation();

	/**
	 *
	 * @return
	 */
	string toString();

	/**
	 *
	 * @return
	 */
	inline double getX() {return m_x;}

	/**
	 *
	 * @return
	 */
	inline double getY() {return m_y;}

	/**
	 *
	 * @return
	 */
	inline double getZ() {return m_z;}

	/**
	 *
	 * @return
	 */
	inline double getSdX() {return m_sdx;}

	/**
	 *
	 * @return
	 */
	inline double getSdY() {return m_sdy;}

	/**
	 *
	 * @return
	 */
	inline double getSdZ() {return m_sdz;}

private:
	double m_x, m_y, m_z;
	double m_sdx, m_sdy, m_sdz;
};

#endif /* INCLUDE_HOMEWORKLOCATION_H_ */
