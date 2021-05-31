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
 * ManhattanDisplacement.h
 *
 *  Created on: May 27, 2021
 *      Author: Bogdan Oancea
 *      Email: bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_MANHATTANDISPLACEMENT_H_
#define INCLUDE_MANHATTANDISPLACEMENT_H_

#include <Displace.h>
#include <Directions.h>
#include <parsers/ManhattanScenario.h>

class ManhattanDisplacement: public Displace {
public:
	/**
	 *
	 * @param simconfig
	 * @param speed
	 * @param id
	 */
	ManhattanDisplacement(SimulationConfiguration* simconfig, double speed, unsigned long id);

	/**
	 *
	 */
	virtual ~ManhattanDisplacement();

	/**
	 *
	 * @param initLocation
	 * @return
	 */
	virtual Point* generateNewLocation(Point * initLocation) override;


private:
	enum STATE {OUTSIDE=0, ONCORNER, ONEDGE};

	Coordinate closestCorner(Coordinate location) const;
	//virtual Point* computeNewLocation(Point* initLocation, double theta) override;

	Directions selectDirection() const;
	bool atCorner(Coordinate pos, Coordinate corner) const;
	bool doubleCompare(double x, double y) const;

	Coordinate makeBlockStep(Coordinate location,  Directions angle) const;
	Coordinate makeIncompleteStep(Coordinate location,  Directions angle, double length) const;
	double makeMultipleBlocksStep(Coordinate& current, double distanceToGo, Directions& angle, ManhattanDisplacement::STATE& status);
	void startFromCorner(Coordinate& current, double distanceToGo, Directions& angle, ManhattanDisplacement::STATE& status);
	void startFromEdge(Coordinate& current, double distanceToGo, Directions& angle, ManhattanDisplacement::STATE& status);
	double blockLength( Directions theta) const;
	Directions reverseDirection(Directions dir) const;

	//STATE checkStatus(Point* location) const;
	ManhattanScenario* m_manhattanScenario;
	STATE m_status;
	double m_distance;
	unsigned long m_id;
	Directions m_theta;

};

#endif /* INCLUDE_MANHATTANDISPLACEMENT_H_ */
