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

/**
 * This class implements the Manhattan mobility pattern. It first computes the closest corner of the grid to the initial location
 * of the person. The first step moves the person between the initial location on map and this corner.
 * Then it computes the distance a person can travel in a time step considering the travel speed.
 * At each time step, a new position is computed as follows. If the starting location is a corner of the Manhattan grid, it randomly select
 * a direction to move. Then, while the distance to go is greater than zero, it checks if the Manhattan step length in the selected direction
 * is less distance to go and if yes moves the person to the next corner, decreasing the distance to go by the step length. When the distance to go
 * is less than the Manhattan step, it moves the person toward the next corner using the selected direction and it checks if it is close enough to this corner. "Close enough"
 * means the the distance to go in one time step is less than the Manhattan step. If it is close enough, it moves the person directly to the corner, otherwise
 * it leaves the person on the edge between two corners.
 * If the starting location is on the edge between two corners, while the distance to go is greater than zero, it checks if the Manhattan step length in the selected direction
 * is less distance to go and if yes moves the person to with a Manhattan step length along the selected direction, decreasing the distance to go by the step length. When the
 * location of the person is close enough to a corner it moves the person directly to the corner. If the distance to go
 * is less than the Manhattan step, it moves the person toward the next corner using the selected direction and it checks if it is close enough to this corner. If it is close enough, it moves the person directly to the corner, otherwise
 * it leaves the person on the edge between two corners.
 * If the new position is outside the map, the current location is kept unchanged, but a new direction for the next move is selected.
 */
class ManhattanDisplacement: public Displace {
public:
	/**
	 * This is the constructor of the class. It stores internally the SimulationConfiguration object and the speed of the person.
	 * @param simconfig a pointer to the SimulationConfiguration object
	 * @param speed the speed of the person.
	 */
	ManhattanDisplacement(SimulationConfiguration* simconfig, double speed);

	/**
	 * Default destructor.
	 */
	virtual ~ManhattanDisplacement();

	/**
	 * Computes the new location after a time step. For details on how this location is computes see the explanation given for this class.
	 * @param initLocation a pointer to a Point object which is the current location.
	 * @return the new location after a time step.
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
	Directions m_theta;

};

#endif /* INCLUDE_MANHATTANDISPLACEMENT_H_ */
