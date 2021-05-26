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
 * MahattanScenario.h
 *
 *  Created on: May 20, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PARSERS_MANHATTANSCENARIO_H_
#define INCLUDE_PARSERS_MANHATTANSCENARIO_H_

/**
 * This class contains the parameters of the Manhattan mobility pattern.
 */
class ManhattanScenario {
public:
	/**
	 * Default constructor.
	 */
	ManhattanScenario();

	/**
	 * Default destructor.
	 */
	virtual ~ManhattanScenario();

	/**
	 * Returns the dimension of the x step of the Manhattan grid.
	 * @return the the dimension of the x step of the Manhattan grid.
	 */
	double getXStep() const;

	/**
	 * Sets the dimension of the x step of the Manhattan grid.
	 * @param xs the dimension of the x step of the Manhattan grid.
	 */
	void setXStep(double xs);

	/**
	 * Returns the dimension of the y step of the Manhattan grid.
	 * @return the dimension of the y step of the Manhattan grid.
	 */
	double getYStep() const;

	/**
	 * Returns the dimension of the y step of the Manhattan grid.
	 * @param ys the dimension of the y step of the Manhattan grid.
	 */
	void setYStep(double ys);

	/**
	 * Returns the x coordinate of the origin of the Manhattan grid.
	 * @return the x coordinate of the origin of the Manhattan grid.
	 */
	double getXOrigin() const;

	/**
	 * Sets the x coordinate of the origin of the Manhattan grid.
	 * @param xOrigin the x coordinate of the origin of the Manhattan grid.
	 */
	void setXOrigin(double xOrigin);

	/**
	 * Returns the y coordinate of the origin of the Manhattan grid.
	 * @return the y coordinate of the origin of the Manhattan grid.
	 */
	double getYOrigin() const;

	/**
	 * Sets the y coordinate of the origin of the Manhattan grid.
	 * @param yOrigin the y coordinate of the origin of the Manhattan grid.
	 */
	void setYOrigin(double yOrigin);


private:
	double m_Xstep;
	double m_Ystep;
	double m_XOrigin;
	double m_YOrigin;
};

#endif /* INCLUDE_PARSERS_MANHATTANSCENARIO_H_ */
