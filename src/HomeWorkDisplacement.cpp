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
#include <parsers/SimConfigParser.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <cmath>
#include <vector>

using namespace std;

HomeWorkDisplacement::HomeWorkDisplacement(SimConfigParser *simConfig, double speed, Point *homeLocation, Point* workLocation) :
		Displace(simConfig, speed) {
	m_deltaTStayHome = initDeltaTStayHome();
	m_deltaTStayWork = initDeltaTStayWork();
	m_state = HomeWorkState::STAY_HOME;
	m_homeLocation = homeLocation;
	m_workLocation = workLocation;
	m_angleDistribution = simConfig->getHomeWorkScenario()->getAngleDistribution();

	//compute estimated speed
	double dist = m_homeLocation->distance(m_workLocation);
	double est_speed = dist / ((simConfig->getEndTime()-simConfig->getStartTime()) * simConfig->getHomeWorkScenario()->getPrecentTimeTravel());
	//cout << "dist to work "  << dist << " speed to work " << est_speed << " time to work: " << ((simConfig->getEndTime()-simConfig->getStartTime()) * simConfig->getHomeWorkScenario()->getPrecentTimeTravel())<< endl;
	m_speed = est_speed;
	m_stepLength = (m_speed / 10.0) * m_simConfig->getClock()->getIncrement();
	std::pair<const char*, double> p1 = std::make_pair("min", 0);
	std::pair<const char*, double> p2 = std::make_pair("max", 2 * utils::PI);
	vector<pair<const char *, double>> params;
	params.push_back(p1);
	params.push_back(p2);
	m_uniform = new Distribution(DistributionType::UNIFORM, params);
}

HomeWorkDisplacement::~HomeWorkDisplacement() {
	if(m_uniform)
		delete m_uniform;
}

Point* HomeWorkDisplacement::generateNewLocation(Point *initLocation) {
	Point* pt;
	switch (m_state) {
	case HomeWorkState::STAY_HOME:
		pt = initLocation;
		break;
	case HomeWorkState::GO_WORK:
		//make a step toward the work location
		pt = toDestination(initLocation, m_workLocation);
		break;
	case HomeWorkState::STAY_WORK:
		pt = makeRandomStepAtWork(initLocation);
		break;
	case HomeWorkState::GO_HOME:
		//make a step toward home location
		pt = toDestination(initLocation, m_homeLocation);
		break;
	default:
		pt = initLocation;
	}
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
		if (posAtDestination(position, m_workLocation))
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
		if (posAtDestination(position, m_homeLocation))
			result = HomeWorkState::STAY_HOME;
		break;
	}
	return result;
}

unsigned long HomeWorkDisplacement::initDeltaTStayHome() const {
	unsigned long simulationTime = m_simConfig->getEndTime() - m_simConfig->getStartTime();
	return m_simConfig->getHomeWorkScenario()->getPrecentTimeHome() * simulationTime;
}

unsigned long HomeWorkDisplacement::initDeltaTStayWork() const {
	unsigned long simulationTime = m_simConfig->getEndTime() - m_simConfig->getStartTime();
	return m_simConfig->getHomeWorkScenario()->getPrecentTimeWork()* simulationTime;
}

const bool HomeWorkDisplacement::posAtDestination(Point* position, Point* destination) const {
	bool result = false;
	if (destination != nullptr)
		result = position->equals(destination);
	else
		result = false;

	return result;

}

Point* HomeWorkDisplacement::makeRandomStepAtWork(Point *initLocation) {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance()->generateDouble(m_uniform);
	double newX = initLocation->getX() + m_stepLength * cos(theta);
	double newY = initLocation->getY() + m_stepLength * sin(theta) ;
	Coordinate c1 = Coordinate(newX, newY, initLocation->getZ());
	Point* pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
	Geometry *g = m_simConfig->getMap()->getBoundary();
	if (!pt->within(g)) {
		int k = 10;
		while (--k && !pt->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(pt);
			theta = RandomNumberGenerator::instance()->generateDouble(m_uniform);
			double newX = initLocation->getX() + m_stepLength * cos(theta);
			double newY = initLocation->getY() + m_stepLength * sin(theta) ;
			Coordinate c1 = Coordinate(newX, newY, initLocation->getZ());
			pt = m_simConfig->getMap()->getGlobalFactory()->createPoint(c1);
		}
		if (!k)
			pt = initLocation;
	}
	return pt;
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
		result = 2.0*utils::PI - abs(atan((y2-y1)/(x2-x1)));

	return result;
}



Point* HomeWorkDisplacement::toDestination(Point*  initLocation, Point* destination) {
	Point* pt;
	double theta = computeTheta(initLocation, destination);
	DistributionType dType = m_angleDistribution->getType();
	double eps = 0.0;
	switch(dType) {
	case DistributionType::LAPLACE:
//		const char* paramName = "scale";
//		double s = m_angleDistribution->getParam(paramName);
		eps = utils::PI* RandomNumberGenerator::instance()->generateDouble(m_angleDistribution) / 180.0;

		//cout << eps << endl;
		break;
	}
	theta += eps;
	pt = computeNewLocation(initLocation, theta);
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
}

const bool HomeWorkDisplacement::arrivedAtDestination(Point* position, Point* destination) const {
	bool result = false;
	double dist = sqrt(pow((position->getX() - destination->getX()), 2)	+ pow((position->getY() - destination->getY()), 2) + pow((position->getZ() - destination->getZ()), 2));
	// allowable dist is a step length
	double allowableDist = m_speed * m_simConfig->getClock()->getIncrement();
	if (dist < allowableDist) {
		result = true;
	}
	return result;
}
