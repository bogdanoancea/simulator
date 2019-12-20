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
 * RandomWalkDisplacement.h
 *
 *  Created on: Nov 20, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <RandomWalkDisplacement.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>

using namespace utils;

class Person;

RandomWalkDisplacement::RandomWalkDisplacement(Map* map, Clock* clk,  double speed):
		Displace(map, clk, speed) {
}

RandomWalkDisplacement::~RandomWalkDisplacement() {
}

Point* RandomWalkDisplacement::generateNewLocation(Point* initLocation) {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
	Point* pt = computeNewLocation(initLocation, theta);

	Geometry* g = m_map->getBoundary();
	if (!pt->within(g))
		pt = initLocation;

	return pt;
}
