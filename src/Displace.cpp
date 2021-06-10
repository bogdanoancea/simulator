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
 * Displace.h
 *
 *  Created on: Nov 20, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */


#include <Displace.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <cmath>

using namespace geos;
using namespace geos::geom;


Displace::Displace(SimulationConfiguration* simConfig, double speed): m_speed {speed} {
	m_simConfig = simConfig;
}

Displace::~Displace() {
}

Point* Displace::computeNewLocation(Point* initLocation, double theta) {
	double x = initLocation->getX();
	double y = initLocation->getY();

	unsigned long delta_t = m_simConfig->getClock()->getIncrement();
	double newX = x + m_speed * cos(theta) * delta_t;
	double newY = y + m_speed * sin(theta) * delta_t;
	Coordinate c1 = Coordinate(newX, newY, initLocation->getZ());
	Point* pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
	return pt;
}

double Displace::getSpeed() {
	return m_speed;
}
