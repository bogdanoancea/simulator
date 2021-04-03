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
	m_workLocationIndex = workLocationIndex;
	m_workLocation = generateWorkLocation();
}

HomeWorkDisplacement::~HomeWorkDisplacement() {
	if(m_workLocation) {
		m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(m_workLocation);
	}

}

Point* HomeWorkDisplacement::generateNewLocation(Point *initLocation) {
	Point* pt;
	switch (m_state) {
	case HomeWorkState::STAY_HOME:
		pt = initLocation;
		break;
	case HomeWorkState::GO_WORK:
		//make a step toward the work location
		pt = toWork(initLocation);
		break;
	case HomeWorkState::STAY_WORK:
		pt = makeRandomStepAtWork(initLocation);
		//pt = initLocation;
		break;
	case HomeWorkState::GO_HOME:
		//make a step toward home location
		pt = toHome(initLocation);
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
	double stepLength = 0.01 * min(wl.m_sdx , wl.m_sdy);
	double newX = initLocation->getX() + stepLength * cos(theta);
	double newY = initLocation->getY() + stepLength * sin(theta) ;
	Coordinate c1 = Coordinate(newX, newY, initLocation->getZ());
	Point* pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		//cout << "am cazut afara" << endl;
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
	Point* pt;
	double theta = computeTheta(initLocation, m_workLocation);
	theta += RandomNumberGenerator::instance()->generateUniformDouble(- utils::PI/20.0, utils::PI/20.0);
	//m_theta = computeTheta(initLocation, m_workLocation);
	//m_theta2 = 0;//RandomNumberGenerator::instance()->generateUniformDouble(- utils::PI/20.0, utils::PI/20.0);
	//m_theta +=m_theta2;
	//pt = computeNewLocation(initLocation, m_theta);
	pt = computeNewLocation(initLocation, theta);
	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		while (--k && !pt->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
			theta = RandomNumberGenerator::instance()->generateUniformDouble(0, utils::PI * 2);
			//m_theta = RandomNumberGenerator::instance()->generateUniformDouble(0, utils::PI * 2);
			pt = computeNewLocation(initLocation, theta);
			//cout << "am cazut afara de la munca: " << k << endl;
		}
		if (!k) {
			pt = initLocation;
			//cout << "am cazut afara de la munca si stau pe loc " << m_theta << ":" << initLocation->toString() <<":" <<m_workLocation->toString() << endl;
		}
	}
	if(arrivedAtWork(pt)) {
		//cout << "am ajuns la munca" << endl;
		pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(m_workLocation->getCoordinates());
	}
	return pt;
}

bool HomeWorkDisplacement::arrivedAtWork(Point* position) {
	bool result = false;
	//HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(m_workLocationIndex);
	double dist = sqrt(pow((position->getX() - m_workLocation->getX()), 2)	+ pow((position->getY() - m_workLocation->getY()), 2) + pow((position->getZ() - m_workLocation->getZ()), 2));
	// allowable dist is a step length
	double allowableDist = m_speed * m_simConfig->getClock()->getIncrement();
	if (dist < allowableDist) {
		result = true;
	}
	return result;
}

bool HomeWorkDisplacement::arrivedAtHome(Point* position) {
	bool result = false;
	//HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(m_workLocationIndex);
	double dist = sqrt(pow((position->getX() - m_homeLocation->getX()), 2)	+ pow((position->getY() - m_homeLocation->getY()), 2) + pow((position->getZ() - m_homeLocation->getZ()), 2));
	// allowable dist is a step length
	double allowableDist = m_speed * m_simConfig->getClock()->getIncrement();
	if (dist < allowableDist) {
		result = true;
	}
	return result;
}

//void HomeWorkDisplacement::setPosAtWork(Point* pt) {
//	if(!m_workLocation) {
//		//cout << "am setat positia la munca" << endl;
//		m_workLocation = m_simConfig->getMap()->getGlobalFactory()->createPoint(pt->getCoordinates());
//	}
//}


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


Point* HomeWorkDisplacement::generateWorkLocation() {
	Point* result = nullptr;
	double angle = RandomNumberGenerator::instance()->generateUniformDouble(0, 2.0 * utils::PI);
	HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(m_workLocationIndex);
	double L = sqrt( 1.0/(pow(cos(angle),2)/wl.m_sdx + pow(sin(angle),2)/wl.m_sdy) );
	double l = RandomNumberGenerator::instance()->generateUniformDouble(0, L);
	double x = wl.m_x + l * cos(angle);
	double y = wl.m_y + l * sin(angle);
	Coordinate c1(x, y, wl.m_z);
	result = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);

	return result;
}

//Point* HomeWorkDisplacement::getWorkLocation() {
//	return m_workLocation;
//}

Point* HomeWorkDisplacement::toHome(Point *initLocation) {
	Point* pt;
	double theta = computeTheta(initLocation, m_homeLocation);
	theta += RandomNumberGenerator::instance()->generateUniformDouble(- utils::PI/20.0, utils::PI/20.0);
	//m_theta = computeTheta(initLocation, m_workLocation);
	//m_theta2 = 0;//RandomNumberGenerator::instance()->generateUniformDouble(- utils::PI/20.0, utils::PI/20.0);
	//m_theta +=m_theta2;
	//pt = computeNewLocation(initLocation, m_theta);
	pt = computeNewLocation(initLocation, theta);
	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		while (--k && !pt->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
			theta = RandomNumberGenerator::instance()->generateUniformDouble(0, utils::PI * 2);
			//m_theta = RandomNumberGenerator::instance()->generateUniformDouble(0, utils::PI * 2);
			pt = computeNewLocation(initLocation, theta);
			//cout << "am cazut afara de la munca: " << k << endl;
		}
		if (!k) {
			pt = initLocation;
			//cout << "am cazut afara de la munca si stau pe loc " << m_theta << ":" << initLocation->toString() <<":" <<m_workLocation->toString() << endl;
		}
	}
	if(arrivedAtHome(pt)) {
		//cout << "am ajuns la munca" << endl;
		pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(m_homeLocation->getCoordinates());
	}
	return pt;
}

