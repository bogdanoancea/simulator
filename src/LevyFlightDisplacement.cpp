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
 * LevyFlightDisplacement.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: Bogdan.Oancea
 */

#include <LevyFlightDisplacement.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>


LevyFlightDisplacement::LevyFlightDisplacement(SimulationConfiguration* simConfig,  double speed):
	Displace(simConfig, speed) {
}

LevyFlightDisplacement::~LevyFlightDisplacement() {
	// TODO Auto-generated destructor stub
}

Point* LevyFlightDisplacement::generateNewLocation(Point* initLocation) {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
	Point* pt = computeNewLocation(initLocation, theta);

	Geometry* g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		while(--k && !pt->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
			theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
			pt = computeNewLocation(initLocation, theta);
		}
		if(!k)
			pt = initLocation;
	}
	return pt;
}

Point* LevyFlightDisplacement::computeNewLocation(Point* initLocation, double theta) {
	double x = initLocation->getX();
	double y = initLocation->getY();
	double speed = RandomNumberGenerator::instance()->generateLevyDouble(m_speed * 0.95, 10);
	if (speed > m_speed *25)
		speed = m_speed *25;
	unsigned long delta_t = m_simConfig->getClock()->getIncrement();
	double newX = x + speed * cos(theta) * delta_t;
	double newY = y + speed * sin(theta) * delta_t;
	Coordinate c1 = Coordinate(newX, newY, 0);
	Point* pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
	return pt;
}
