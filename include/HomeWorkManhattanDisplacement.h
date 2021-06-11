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
 * HomeWorkManhattanDisplacement.h
 *
 *  Created on: Jun 4, 2021
 *      Author: Bogdan Oancea
 *      Email:  bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_
#define INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_

#include <HomeWorkDisplacement.h>
#include <ManhattanDisplacement.h>
#include <Directions.h>



class HomeWorkManhattanDisplacement: public HomeWorkDisplacement {
public:
	HomeWorkManhattanDisplacement(SimulationConfiguration* simConfig, double speed, Point* homeLocation, Point* workLocation, Point* anchorLocation);
	virtual ~HomeWorkManhattanDisplacement();
	virtual Point* generateNewLocation(Point * initLocation) override;

private:
	virtual Point* toDestination(Point*  initLocation, Point* destination) override;
	virtual const bool arrivedAtDestination(Point* position, Point* destination) const override;
	virtual Point* makeRandomStepAtWork(Point* initLocation) override;
	ManhattanDisplacement m_manhattanDisplacement;
};

#endif /* INCLUDE_HOMEWORKMANHATTANDISPLACEMENT_H_ */
