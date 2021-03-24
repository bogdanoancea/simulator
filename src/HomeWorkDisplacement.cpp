/*
 * HomeWorkDisplacement.cpp
 *
 *  Created on: Mar 22, 2021
 *      Author: bogdan
 */

#include <Clock.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Coordinate.inl>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <HomeWorkDisplacement.h>
#include <map/Map.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <cmath>

HomeWorkDisplacement::HomeWorkDisplacement(SimConfig* simConfig, double speed): Displace(simConfig, speed) {
	// TODO Auto-generated constructor stub
}


HomeWorkDisplacement::~HomeWorkDisplacement() {
	// TODO Auto-generated destructor stub
}


//TODO
Point* HomeWorkDisplacement::generateNewLocation(Point* initLocation) {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
	Point* pt = computeNewLocation(initLocation, theta);

	Geometry* g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		while(--k && !pt->within(g)) {
			theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
			pt = computeNewLocation(initLocation, theta);
		}
		if(!k)
			pt = initLocation;
	}
	return pt;
}

//TODO
Point* HomeWorkDisplacement::computeNewLocation(Point* initLocation, double theta) {
	double x = initLocation->getX();
	double y = initLocation->getY();
	double speed = RandomNumberGenerator::instance()->generateLevy(m_speed * 0.95, 10);
	if (speed > m_speed *25)
		speed = m_speed *25;
	unsigned long delta_t = m_simConfig->getClock()->getIncrement();
	double newX = x + speed * cos(theta) * delta_t;
	double newY = y + speed * sin(theta) * delta_t;
	Coordinate c1 = Coordinate(newX, newY, 0);
	Point* pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
	return pt;
}
