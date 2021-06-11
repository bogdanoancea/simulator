/*
 * HomeWorkManhattanDisplacement.cpp
 *
 *  Created on: Jun 4, 2021
 *      Author: bogdan
 */

#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <HomeWorkManhattanDisplacement.h>
#include <parsers/SimulationConfiguration.h>
#include <cmath>
#include <Utils.h>

using namespace std;

HomeWorkManhattanDisplacement::HomeWorkManhattanDisplacement(
		SimulationConfiguration *simConfig, double speed, Point *homeLocation,
		Point *workLocation, Point *anchorLocation) :
		HomeWorkDisplacement(simConfig, speed, homeLocation, workLocation, anchorLocation), m_manhattanDisplacement(simConfig, speed) {
	m_speed *= sqrt(2);
	m_manhattanDisplacement.setSpeed(m_speed);
}

HomeWorkManhattanDisplacement::~HomeWorkManhattanDisplacement() {
	// TODO Auto-generated destructor stub
}

Point* HomeWorkManhattanDisplacement::toDestination(Point*  initLocation, Point* destination) {
	Point* pt = nullptr;
	double theta = computeTheta(initLocation, destination);

	if(m_manhattanDisplacement.getStatus() == ManhattanDisplacement::STATE::OUTSIDE) {
		Coordinate c = m_manhattanDisplacement.closestCorner(*initLocation->getCoordinate());
		pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c);
		m_manhattanDisplacement.setStatus(ManhattanDisplacement::STATE::ONCORNER);
		theta = computeTheta(pt, destination);
	}
	m_manhattanDisplacement.setDirection(theta);
	if(pt == nullptr)
		pt = m_manhattanDisplacement.generateNewLocation(initLocation);
	else
		pt = m_manhattanDisplacement.generateNewLocation(pt);
	if (pt->equals(initLocation)) {
		m_manhattanDisplacement.setDirection(-1);
		pt = m_manhattanDisplacement.generateNewLocation(initLocation);
	}
	if(arrivedAtDestination(pt, destination)) {
		pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(destination->getCoordinates());
		m_manhattanDisplacement.setStatus(m_manhattanDisplacement.checkStatus(destination));
	}
	return pt;
}


Point* HomeWorkManhattanDisplacement::generateNewLocation(Point * initLocation) {
	Point* result;
	result =  HomeWorkDisplacement::generateNewLocation(initLocation);
	return result;
}

const bool HomeWorkManhattanDisplacement::arrivedAtDestination(Point* position, Point* destination) const {
	bool result = false;
	double dist = sqrt(pow((position->getX() - destination->getX()), 2)	+ pow((position->getY() - destination->getY()), 2) + pow((position->getZ() - destination->getZ()), 2));
	// allowable dist is a step length
	double allowableDist = m_speed * m_simConfig->getClock()->getIncrement();
	if (dist < allowableDist) {
		result = true;
	} else	{
		ManhattanScenario* mht = static_cast<HomeWorkManhattanScenario*>(m_simConfig->getHomeWorkManhattanScenario())->getManhattanScenario();
		const Coordinate* dc = destination->getCoordinate();
		const Coordinate* posc = position->getCoordinate();
		double x1 = floor((dc->x - mht->getXOrigin()) / mht->getXStep()) * mht->getXStep();
		double x2 = ceil((dc->x - mht->getXOrigin()) / mht->getXStep()) * mht->getXStep();
		double y1 = floor((dc->y - mht->getYOrigin()) / mht->getYStep()) * mht->getYStep();
		double y2 = ceil((dc->y - mht->getYOrigin()) / mht->getYStep()) * mht->getYStep();
		if( (posc->y == y1 || posc->y == y2) && (posc->x >= x1 && posc->x <= x2))
			result = true;
		else if ((posc->x == x1 || posc->x == x2) && (posc->y >= y1 && posc->y <= y2))
			result = true;
	}
	return result;

}


Point* HomeWorkManhattanDisplacement::makeRandomStepAtWork(Point* initLocation) {
	return initLocation;
}

