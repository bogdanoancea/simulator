/*
 * RandomWalkDriftDisplacement.cpp
 * Data simulator for mobile phone network events
 *  Created on: Nov 21, 2019
 *      Author: bogdanoancea
 */

#include <RandomWalkDriftDisplacement.h>
#include <RandomNumberGenerator.h>
#include <Constants.h>
#include <Utils.h>

using namespace utils;

RandomWalkDriftDisplacement::RandomWalkDriftDisplacement(Map* map, Clock* clk, double speed):
		Displace(map, clk, speed), m_changeDirection{false} {
}

RandomWalkDriftDisplacement::~RandomWalkDriftDisplacement() {
}

Point* RandomWalkDriftDisplacement::generateNewLocation(Point* initLocation) {

	double theta = 0.0;
	double trendAngle = Constants::SIM_TREND_ANGLE_1;

	if (m_clock->getCurrentTime() >= m_clock->getFinalTime() / 2) {
		trendAngle = Constants::SIM_TREND_ANGLE_2;
	}
	theta = RandomNumberGenerator::instance()->generateNormalDouble(trendAngle, 0.1);
	if (m_changeDirection) {
		theta = theta + utils::PI / RandomNumberGenerator::instance()->generateUniformDouble(0.5, 1.5);
	}
	Point* pt = computeNewLocation(initLocation, theta);

	Geometry* g = m_map->getBoundary();
	if (!pt->within(g)) {
		m_changeDirection = !m_changeDirection;
		pt = initLocation;
	}

	return pt;
}
