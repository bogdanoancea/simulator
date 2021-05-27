/*
 * ManhattanDisplacement.cpp
 *
 *  Created on: May 27, 2021
 *      Author: bogdan
 */

#include <ManhattanDisplacement.h>
#include <RandomNumberGenerator.h>

ManhattanDisplacement::ManhattanDisplacement(SimulationConfiguration *simConfig, double speed) :
		Displace(simConfig, speed) {
	m_manhattanScenario = simConfig->getManhattanScenario();
}

ManhattanDisplacement::~ManhattanDisplacement() {
	// TODO Auto-generated destructor stub
}

Point* ManhattanDisplacement::generateNewLocation(Point * initLocation) {
	Point* result = initLocation;
	double xstep  = m_manhattanScenario->getXStep();
	double ystep  = m_manhattanScenario->getYStep();

	if(m_simConfig->getClock()->getCurrentTime() == m_simConfig->getClock()->getInitialTime() +  m_simConfig->getClock()->getIncrement()) {
		//cout << "trec pe aici" << endl;
		result = closestCorner(initLocation);
	} else {
		double distance = m_speed * m_simConfig->getClock()->getIncrement();
		double x = result->getX();
		double y = result->getY();

		double maxstep = xstep >= ystep ? xstep : ystep;
		while(distance > maxstep) {
			unsigned theta = selectDirection();
			if( theta == 0) {
				x += xstep;
				distance -=xstep;
			}
			else if(theta == 90) {
				y += ystep;
				distance -=ystep;
			}
			else if(theta == 180) {
				x -= xstep;
				distance -=xstep;
			}
			else if(theta == 270) {
				y -= ystep;
				distance -=ystep;
			}

		}
		if(maxstep == xstep) // make a step on OY
			y += ystep;
		else
			x += xstep;
		Coordinate c1 = Coordinate(x, y, 0);
		result = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
		Geometry* g = m_simConfig->getMap()->getBoundary();
		if (!result->within(g))
			result = initLocation;
	}

	return result;

}

Point* ManhattanDisplacement::closestCorner(Point* location) const {
	double xcorner, ycorner;
	double x = location->getX();
	double y = location->getY();

	double x1 = floor((x - m_manhattanScenario->getXOrigin()) / m_manhattanScenario->getXStep()) * m_manhattanScenario->getXStep();
	double x2 = ceil((x - m_manhattanScenario->getXOrigin()) / m_manhattanScenario->getXStep()) * m_manhattanScenario->getXStep();
	double y1 = floor((y - m_manhattanScenario->getYOrigin()) / m_manhattanScenario->getYStep()) * m_manhattanScenario->getYStep();
	double y2 = ceil((y - m_manhattanScenario->getYOrigin()) / m_manhattanScenario->getYStep()) * m_manhattanScenario->getYStep();
	if(abs(x-x1) < abs(x-x2))
		xcorner = x1;
	else
		xcorner = x2;

	if(abs(y-y1) < abs(y-y2))
		ycorner = y1;
	else
		ycorner = y2;

	Coordinate c1 = Coordinate(xcorner, ycorner, 0);
	Point* pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
	return pt;
}

unsigned ManhattanDisplacement::selectDirection() {
	RandomNumberGenerator* rng = RandomNumberGenerator::instance();
	unsigned theta = rng->generateUniformInt(0, 3);
	return theta * 90;
}


//
//Point* LevyFlightDisplacement::computeNewLocation(Point* initLocation, double theta) {
//	double x = initLocation->getX();
//	double y = initLocation->getY();
//	return pt;
//}
