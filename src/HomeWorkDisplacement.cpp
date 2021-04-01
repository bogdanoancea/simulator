/*
 * HomeWorkDisplacement.cpp
 *
 *  Created on: Mar 22, 2021
 *      Author: bogdan
 */

#include <Clock.h>
#include <geos/geom/Point.h>
#include <HomeWorkDisplacement.h>
#include <map/Map.h>
#include <parsers/HomeWorkLocation.h>
#include <parsers/HomeWorkScenario.h>
#include <parsers/SimConfig.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <cmath>
#include <vector>

using namespace std;

HomeWorkDisplacement::HomeWorkDisplacement(SimConfig *simConfig, double speed, Point *homeLocation, unsigned int workLocationIndex) :
		Displace(simConfig, speed) {
	m_deltaTStayHome = initDeltaTStayHome();
	m_deltaTStayWork = initDeltaTStayWork();
	m_state = HomeWorkState::STAY_HOME;
	m_homeLocation = homeLocation;
	m_workLocation = nullptr;
	m_workLocationIndex = workLocationIndex;
}

HomeWorkDisplacement::~HomeWorkDisplacement() {
	// TODO Auto-generated destructor stub
}

//TODO
Point* HomeWorkDisplacement::generateNewLocation(Point *initLocation) {
	Point* pt;
	//	double theta = 0.0;
//	theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
//	Point *pt = computeNewLocation(initLocation, theta);
//
//	Geometry *g = m_simConfig->getMap()->getBoundary();
//	if (!pt->within(g)) {
//		int k = 10;
//		while (--k && !pt->within(g)) {
//			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
//			theta = RandomNumberGenerator::instance()->generateUniformDouble( 0.0, 2 * utils::PI);
//			pt = computeNewLocation(initLocation, theta);
//		}
//		if (!k)
//			pt = initLocation;
//	}

	switch (m_state) {
	case HomeWorkState::STAY_HOME:
		pt = initLocation;
		break;
	case HomeWorkState::GO_WORK:
		//make a step toward the work location
		pt = toWork(initLocation);
		break;
	case HomeWorkState::STAY_WORK:
		//pt = makeRandomStepAtWork(initLocation);
		pt = initLocation;
		break;
	case HomeWorkState::GO_HOME:
		//make a step toward home location
		pt = initLocation;
		break;
	default:
		pt = initLocation;
	}
	if (m_homeLocation)
		m_state = stateTransition(pt);

	return pt;
}


HomeWorkState HomeWorkDisplacement::stateTransition(Point *position) {
	HomeWorkState result = m_state;
	switch (m_state) {
	case HomeWorkState::STAY_HOME:
		//cout << " state: " << static_cast<int>(m_state) << endl;
		m_deltaTStayHome -= m_simConfig->getClock()->getIncrement();
		if (m_deltaTStayHome <= 0) {
			result = HomeWorkState::GO_WORK;
			m_deltaTStayHome = initDeltaTStayHome();
		}
		break;
	case HomeWorkState::GO_WORK:
		//cout << " state: " << static_cast<int>(m_state) << endl;
		if (posAtWork(position))
			result = HomeWorkState::STAY_WORK;
		break;
	case HomeWorkState::STAY_WORK:
		//cout << " state: STAY_WORK "  << endl;
		m_deltaTStayWork -= m_simConfig->getClock()->getIncrement();
		if (m_deltaTStayWork <= 0) {
			result = HomeWorkState::GO_HOME;
			m_deltaTStayWork = initDeltaTStayWork();
			//cout << " STAY_WORK time: " <<m_deltaTStayWork << endl;
		}
		break;
	case HomeWorkState::GO_HOME:
		//cout << " state: " << static_cast<int>(m_state) << endl;
		if (posAtHome(position))
			result = HomeWorkState::STAY_HOME;
		break;
	}
	return result;
}

unsigned long HomeWorkDisplacement::initDeltaTStayHome() {
	unsigned long simulationTime = m_simConfig->getEndTime() - m_simConfig->getStartTime();
	return m_simConfig->getHomeWorkScenario()->getPrecentTimeHome() * simulationTime;
}

unsigned long HomeWorkDisplacement::initDeltaTStayWork() {
	unsigned long simulationTime = m_simConfig->getEndTime() - m_simConfig->getStartTime();
	return m_simConfig->getHomeWorkScenario()->getPrecentTimeWork()* simulationTime;
}

bool HomeWorkDisplacement::posAtHome(Point *position) const {
	bool result = false;
	if (m_homeLocation != nullptr)
		result = (position->equals(m_homeLocation));
	else
		result = false;

	return result;
}

bool HomeWorkDisplacement::posAtWork(Point *position) {
	bool result = false;
	if (m_workLocation != nullptr)
		result = position->equals(m_workLocation);
	else
		result = false;

	return result;
}

Point* HomeWorkDisplacement::makeRandomStepAtWork(Point *initLocation) {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
	HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(m_workLocationIndex);
	double stepLength = 0.1 * min(wl.m_sdx , wl.m_sdy);
	double newX = initLocation->getX() + stepLength * cos(theta);
	double newY = initLocation->getY() + stepLength * sin(theta) ;
	Coordinate c1 = Coordinate(newX, newY, initLocation->getZ());
	Point* pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		cout << "am cazut afara" << endl;
		while (--k && !pt->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
			theta = RandomNumberGenerator::instance()->generateUniformDouble( 0.0, 2 * utils::PI);
			double newX = initLocation->getX() + stepLength * cos(theta);
			double newY = initLocation->getY() + stepLength * sin(theta) ;
			Coordinate c1 = Coordinate(newX, newY, initLocation->getZ());
			pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
		}
		if (!k)
			pt = initLocation;
	}
	return pt;
}

Point* HomeWorkDisplacement::toWork(Point *initLocation) {
	Point *pt, *workLoc;

	if(m_workLocation)
		workLoc = m_workLocation;
	else {
		HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(m_workLocationIndex);
		workLoc = m_simConfig->getMap()->getGlobalFactory()->createPoint(Coordinate(wl.m_x, wl.m_y, wl.m_z));
	}

	double theta = computeTheta(initLocation, workLoc);
	//no need of workLoc anymore

	theta += RandomNumberGenerator::instance()->generateUniformDouble(- utils::PI/16.0, utils::PI/16.0);
	pt = computeNewLocation(initLocation, theta);
	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {

		int k = 10;
		while (--k && !pt->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
			theta = computeTheta(initLocation, workLoc);
			theta += RandomNumberGenerator::instance()->generateUniformDouble(- utils::PI/16.0, utils::PI/16.0);
			pt = computeNewLocation(initLocation, theta);
			//cout << "am cazut afara de la munca: " << k << endl;
		}
		if (!k) {
			pt = initLocation;
			//cout << "am cazut afara de la munca si stau pe loc " << endl;
		}
	}
	if(arrivedAtWork(pt)) {
		//cout << "am ajuns la munca" << endl;
		setPosAtWork(pt);
	}
	if(!m_workLocation)
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(workLoc);
	return pt;
}

bool HomeWorkDisplacement::arrivedAtWork(Point* position) {
	bool result = false;
	HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(m_workLocationIndex);
	double dist = sqrt(pow((position->getX() - wl.m_x), 2)	+ pow((position->getY() - wl.m_y), 2) + pow((position->getZ() - wl.m_z), 2));
	double allowableDist = sqrt(pow(wl.m_sdx, 2) + pow(wl.m_sdy, 2) + pow(wl.m_sdz, 2));
	if (dist < allowableDist) {
		result = true;
	}
	return result;
}


void HomeWorkDisplacement::setPosAtWork(Point* pt) {
	if(!m_workLocation) {
		//cout << "am setat positia la munca" << endl;
		m_workLocation = m_simConfig->getMap()->getGlobalFactory()->createPoint(pt->getCoordinates());
	}
}


double HomeWorkDisplacement::computeTheta(Point* p1, Point* p2) const {
	double result = 0.0;
	double x1 = p1->getX();
	double x2 = p2->getX();
	double y1 = p1->getY();
	double y2 = p2->getY();

	if(x2 >= x1 && y2 >= y1)
		result = atan((y2-y1)/(x2-x1));
	if(x2 < x1 && y2 >= y1)
		result = utils::PI - abs(atan((y2-y1)/(x2-x1)));
	if(x2 < x1 && y2 < y1)
		result = atan((y2-y1)/(x2-x1)) + utils::PI;
	if(x2 >= x1 && y2 < y1)
		result = 3.0*utils::PI/2.0 + abs(atan((y2-y1)/(x2-x1)));

	return result;
}

