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
}

HomeWorkDisplacement::~HomeWorkDisplacement() {
	// TODO Auto-generated destructor stub
}

//TODO
Point* HomeWorkDisplacement::generateNewLocation(Point *initLocation) {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
	Point *pt = computeNewLocation(initLocation, theta);

	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		while (--k && !pt->within(g)) {
			theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
			pt = computeNewLocation(initLocation, theta);
		}
		if (!k)
			pt = initLocation;
	}
	m_state = stateTransition(pt);

	return pt;
}

//TODO
Point* HomeWorkDisplacement::computeNewLocation(Point *initLocation, double theta) {
	return initLocation;
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
		//cout << " state: " << static_cast<int>(m_state) << endl;
		m_deltaTStayWork -= m_simConfig->getClock()->getIncrement();
		if (m_deltaTStayWork <= 0) {
			result = HomeWorkState::GO_HOME;
			m_deltaTStayWork = initDeltaTStayWork();
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
	return m_simConfig->getHomeWorkScenario()->getPrecentTimeHome()
			* simulationTime;
}

unsigned long HomeWorkDisplacement::initDeltaTStayWork() {
	unsigned long simulationTime = m_simConfig->getEndTime() - m_simConfig->getStartTime();
	return m_simConfig->getHomeWorkScenario()->getPrecentTimeWork()
			* simulationTime;
}

bool HomeWorkDisplacement::posAtHome(Point *position) const {
	bool result = false;

	if(m_homeLocation != nullptr)
		result = (position->equals(m_homeLocation));
	else
		result = false;

	return result;
}

bool HomeWorkDisplacement::posAtWork(Point *position) const {
	if (m_homeLocation != nullptr) {
		HomeWorkLocation wl = m_simConfig->getHomeWorkScenario()->getWorkLocations().at(m_workLocationIndex);
		double dist = sqrt(pow((position->getX() - wl.m_x), 2)
						+ pow((position->getY() - wl.m_y), 2)
						+ pow((position->getZ() - wl.m_z), 2));
		double allowableDist = sqrt(pow(wl.m_sdx, 2) + pow(wl.m_sdy, 2) + pow(wl.m_sdz, 2));
		if (dist < allowableDist)
			return true;
		else
			return false;
	} else
		return false;

}
