/*
 * HomeWorkManhattanDisplacement.cpp
 *
 *  Created on: Jun 4, 2021
 *      Author: bogdan
 */

#include <HomeWorkManhattanDisplacement.h>
#include <ManhattanDisplacement.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>


HomeWorkManhattanDisplacement::HomeWorkManhattanDisplacement(
		SimulationConfiguration *simConfig, double speed, Point *homeLocation,
		Point *workLocation, Point *anchorLocation) :
		HomeWorkDisplacement(simConfig, speed, homeLocation, workLocation, anchorLocation), m_manhattanDisplacement(simConfig, speed) {

}

HomeWorkManhattanDisplacement::~HomeWorkManhattanDisplacement() {
	// TODO Auto-generated destructor stub
}

Point* HomeWorkManhattanDisplacement::toDestination(Point*  initLocation, Point* destination) {
	Point* pt;
	double theta = computeTheta(initLocation, destination);
	//cout << "theta este: " << theta << endl;
	if(m_manhattanDisplacement.getStatus() == ManhattanDisplacement::STATE::OUTSIDE) {
		Coordinate c = m_manhattanDisplacement.closestCorner(*initLocation->getCoordinate());
		pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c);
		m_manhattanDisplacement.setStatus(ManhattanDisplacement::STATE::ONCORNER);
	}
	m_manhattanDisplacement.setDirection(theta);
	pt = m_manhattanDisplacement.generateNewLocation(initLocation);
	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		while (--k && !pt->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
			theta = RandomNumberGenerator::instance()->generateUniformDouble(0, utils::PI * 2);
			pt = computeNewLocation(initLocation, theta);
		}
		if (!k) {
			pt = initLocation;
		}
	}
	if(arrivedAtDestination(pt, destination)) {
		pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(destination->getCoordinates());
	}
	return pt;

	//return destination;
}



Point* HomeWorkManhattanDisplacement::generateNewLocation(Point * initLocation) {
	Point* result;
	result =  HomeWorkDisplacement::generateNewLocation(initLocation);
	return result;
}



Directions selectDirectionToDestination(double theta) {
	Directions result =  Directions::EAST;
	RandomNumberGenerator* rng = RandomNumberGenerator::instance();
	if(theta == 0.0)
		result = Directions::EAST;
	else if(theta > 0.0 && theta < utils::PI / 2) {
		unsigned d = rng->generateUniformInt(0, 1);
		if(d == 0)
			result = Directions::EAST;
		else if(d == 1)
			result = Directions::NORTH;
	}
	else if(theta == utils::PI)
		result = Directions::NORTH;
	else if(theta > utils::PI / 2 && theta < utils::PI) {
		unsigned d = rng->generateUniformInt(0, 1);
		if(d == 0)
			result = Directions::NORTH;
		else if(d == 1)
			result = Directions::WEST;
	} else if( theta == utils::PI)
		result = Directions::WEST;
	else if(theta > utils::PI && theta < 3.0 * utils::PI / 2.0) {
		unsigned d = rng->generateUniformInt(0, 1);
		if(d == 0)
			result = Directions::WEST;
		else if(d == 1)
			result = Directions::SOUTH;
	}
	else if(theta == 3.0 * utils::PI / 2.0)
		result = Directions::SOUTH;
	else if(theta > 3.0 * utils::PI / 2.0 && theta < 2 * utils::PI) {
		unsigned d = rng->generateUniformInt(0, 1);
		if(d == 0)
			result = Directions::SOUTH;
		else if(d == 1)
			result = Directions::EAST;
	}
	return result;
}
