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
	 * @param y  the y coordinate of a Home - Work location.
	 * @param z  the z coordinate of a Home - Work location.
	 * @param sdx the standard deviation / radius of the x coordinate of a Home - Work location.
	 * @param sdy the standard deviation / radius of the y coordinate of a Home - Work location.
	 * @param sdz the standard deviation / radius of the z coordinate of a Home - Work location.
	 */
	HomeWorkLocation(double x, double y, double z, double sdx, double sdy, double sdz);

	/**
	 * The default constructor is deleted. A HomeWorkLocation object without the values for the members makes no sense.
	 */
	HomeWorkLocation() = delete;

	/**
	 * Default destructor.
	 */
	virtual ~HomeWorkLocation();

	/**
	 * Return a string representation of the class, containing the values of all members, comma separated. It is useful
	 * for debugging or for printing these information. An example of such a string:
	 *
	 * \code
	 * X:2000 Y:7500 Z:0 Sd_x:1000 Sd_y:1000 Sd_z:0
	 * \endcode
	 *
	 * @return a string representation of the class, containing the values of all members, comma separated.
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
