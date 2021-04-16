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
 * RandomWalkDriftDisplacement.cpp
 *  Created on: Nov 21, 2019
 *      Author: bogdanoancea
 */

#include <RandomWalkDriftDisplacement.h>
#include <RandomNumberGenerator.h>
#include <Constants.h>
#include <Utils.h>

using namespace utils;

RandomWalkDriftDisplacement::RandomWalkDriftDisplacement(SimConfig* simConfig, double speed):
		Displace(simConfig, speed), m_changeDirection{false} {

}

RandomWalkDriftDisplacement::~RandomWalkDriftDisplacement() {
}

Point* RandomWalkDriftDisplacement::generateNewLocation(Point* initLocation) {

	double theta = 0.0;
	double trendAngle = Constants::SIM_TREND_ANGLE_1;
	if (m_simConfig->getClock()->getCurrentTime() >= m_simConfig->getClock()->getFinalTime() / 2) {
		trendAngle = Constants::SIM_TREND_ANGLE_2;
	}

	theta = RandomNumberGenerator::instance()->generateNormalDouble(trendAngle, 0.1);
	if (m_changeDirection) {
		theta = theta + utils::PI / RandomNumberGenerator::instance()->generateUniformDouble(0.5, 1.5);
	}
	Point* pt = computeNewLocation(initLocation, theta);

	Geometry* g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
		m_changeDirection = !m_changeDirection;
		pt = initLocation;
	}

	return pt;
}
