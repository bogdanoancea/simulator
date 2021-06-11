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
 * HomeWorkManhattanScenario.h
 *
 *  Created on: Jun 4, 2021
 *      Author: Bogdan.Oancea
 */

#ifndef INCLUDE_PARSERS_HOMEWORKMANHATTANSCENARIO_H_
#define INCLUDE_PARSERS_HOMEWORKMANHATTANSCENARIO_H_

#include <parsers/HomeWorkScenario.h>
#include <parsers/ManhattanScenario.h>

/**
 * This class contains the parameters of an home-work scenario using a Manhattan mobility pattern. For details about the
 * home-work scenario consul the documentation of the HomeWorkScenario class. This scenario adds the Manhattan mobility
 * pattern between home, work and anchor point location.
 */
class HomeWorkManhattanScenario: public HomeWorkScenario {

public:

	/**
	 * The constructor of the class. It takes a point to a ManhattanScenario object and stores it internally.
	 * It need this object to implement the Manhattan mobility scenario.
	 * @param ms
	 */
	HomeWorkManhattanScenario(ManhattanScenario* ms);

	/**
	 * default destructor.
	 */
	virtual ~HomeWorkManhattanScenario();

	/**
	 *	/**
	 * Returns a string showing all home, work, anchor point locations and the parameters of the Manhattan grid.
	 * @return a string showing all home, work, anchor point locations and the parameters of the Manhattan grid.
	 */
	const string toString() override;

	/**
	 * Returns a pointer to the ManhattanScenario object passed to the constructor.
	 * @return  a pointer to the ManhattanScenario object passed to the constructor.
	 */
	ManhattanScenario* getManhattanScenario();

private:
	ManhattanScenario* m_manhattanScenario;
};

#endif /* INCLUDE_PARSERS_HOMEWORKMANHATTANSCENARIO_H_ */
