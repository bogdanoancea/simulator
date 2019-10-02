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
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <geos/geom/Coordinate.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/Point.h>

#include <Agent.h>
#include <HoldableAgent.h>
#include <Map.h>
#include <Person.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <Constants.h>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

using namespace geos;
using namespace geos::geom;

Person::Person(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed, int age, Gender gen, unsigned long timeStay,
		unsigned long intervalBetweenStays) :
		MovableAgent(m, id, initPosition, clock, initSpeed), m_age { age }, m_gender { gen }, m_changeDirection { false }, m_avgTimeStay { timeStay }, m_avgIntervalBetweenStays {
				intervalBetweenStays } {
	m_nextStay = getClock()->getCurrentTime() + intervalBetweenStays;
	while (m_nextStay % getClock()->getIncrement() != 0)
		m_nextStay++;

	m_timeStay = timeStay;
	while (m_timeStay % getClock()->getIncrement() != 0)
		m_timeStay++;

}

Person::~Person() {
}

int Person::getAge() const {
	return (m_age);
}

void Person::setAge(int age) {
	m_age = age;
}

const string Person::toString() const {
	ostringstream ss;
	ss << MovableAgent::toString() << left << setw(15) << m_age << setw(15) << (m_gender == Person::Gender::MALE ? "Male" : "Female");

	if (m_idDevices.size() > 0) {
		for (pair<string, Agent*> i : m_idDevices) {
			HoldableAgent* h = dynamic_cast<HoldableAgent*>(i.second);
			ss << setw(15) << (h->getId());
		}
	}
	return (ss.str());
}

Point* Person::move(MovementType mvType) {
	unsigned long currentTime = getClock()->getCurrentTime();
	if (currentTime >= m_nextStay && currentTime <= m_nextStay + m_timeStay) {
		setLocation(getLocation());
		if (currentTime == m_nextStay + m_timeStay) {
			unsigned long nextInterval = (unsigned long) RandomNumberGenerator::instance()->generateExponentialDouble(1.0 / this->m_avgIntervalBetweenStays);
			while (nextInterval % getClock()->getIncrement() != 0)
				nextInterval++;

			m_nextStay = currentTime + nextInterval;
			m_timeStay = (unsigned long) RandomNumberGenerator::instance()->generateNormalDouble(m_avgTimeStay, 0.2 * m_avgTimeStay);
			while (m_timeStay % getClock()->getIncrement() != 0)
				m_timeStay++;

		}
	} else if (mvType == MovementType::RANDOM_WALK_CLOSED_MAP)
		randomWalkClosedMap();
	else if (mvType == MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT)
		randomWalkClosedMapDrift();
	else
		throw runtime_error("Movement type not yet implemented");

	return (getLocation());
}

void Person::randomWalkClosedMap() {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance()->generateUniformDouble(0.0, 2 * utils::PI);
	Point* pt = generateNewLocation(theta);
	setNewLocation(pt, false);
}

void Person::randomWalkClosedMapDrift() {
	double theta = 0.0;
	double trendAngle = Constants::SIM_TREND_ANGLE_1;
	if (getClock()->getCurrentTime() >= getClock()->getFinalTime() / 2) {
		trendAngle = Constants::SIM_TREND_ANGLE_2;
	}
	theta = RandomNumberGenerator::instance()->generateNormalDouble(trendAngle, 0.1);
	if (m_changeDirection) {
		theta = theta + utils::PI / RandomNumberGenerator::instance()->generateUniformDouble(0.5, 1.5);
	}
	Point* pt = generateNewLocation(theta);
	setNewLocation(pt, true);
}

Point* Person::generateNewLocation(double theta) {
	double x = getLocation()->getCoordinate()->x;
	double y = getLocation()->getCoordinate()->y;
	double speed = getSpeed();
	unsigned long delta_t = getClock()->getIncrement();
	double newX = x + speed * cos(theta) * delta_t;
	double newY = y + speed * sin(theta) * delta_t;
	Coordinate c = Coordinate(newX, newY, 0);
	Point* pt = getMap()->getGlobalFactory()->createPoint(c);
	return pt;
}

unsigned long Person::getAvgTimeStay() const {
	return (m_avgTimeStay);
}

unsigned long Person::getAvgIntervalBetweenStays() const {
	return (m_avgIntervalBetweenStays);
}

void Person::setNewLocation(Point* p, bool changeDirection) {
	Geometry* g = getMap()->getBoundary();
	if (p->within(g)) {
		this->getMap()->getGlobalFactory()->destroyGeometry(getLocation());
		setLocation(p);
	} else {
		if (changeDirection)
			m_changeDirection = !m_changeDirection;
		setLocation(getLocation());
		/*
		 CoordinateSequence* cl = new CoordinateArraySequence();
		 cl->add(Coordinate(getLocation()->getCoordinate()->x, getLocation()->getCoordinate()->y));
		 cl->add(Coordinate(p->getCoordinate()->x, p->getCoordinate()->y));

		 LineString* ls = this->getMap()->getGlobalFactory()->createLineString(cl);
		 Geometry* intersect = ls->intersection(g);
		 LineString* intersect2 = dynamic_cast<LineString*>(intersect);
		 Point* ptInt = intersect2->getEndPoint();
		 //cout << ptInt->toString()  << endl;
		 if (ptInt) {
		 cout << ptInt->toString()  << endl;
		 if(this->getClock()->getCurrentTime() == 2)
		 cout << "la momentul 2 setez locatia2" << endl;
		 this->getMap()->getGlobalFactory()->destroyGeometry(getLocation());
		 setLocation(ptInt);
		 }
		 this->getMap()->getGlobalFactory()->destroyGeometry(ls);
		 this->getMap()->getGlobalFactory()->destroyGeometry(p);
		 */
	}
}

bool Person::hasDevices() {
	return (m_idDevices.size() > 0);
}

void Person::setLocation(Point* location) {
	LocatableAgent::setLocation(location); //
	if (hasDevices()) {
		unordered_multimap<string, Agent*>::iterator it;
		for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
			Agent* a = it->second;
			HoldableAgent* device = dynamic_cast<HoldableAgent*>(a);
			if (device != nullptr) {
				device->setLocation(location);
			}
		}
	}
}

string Person::dumpDevices() {
	stringstream ss;
	char sep = Constants::sep;
	unordered_multimap<string, Agent*>::iterator it;
	for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
		Agent* a = it->second;
		ss << sep << a->getId();
	}
	return (ss.str());
}

Person::Gender Person::getGender() const {
	return (m_gender);
}

const string Person::getName() const {
	return ("Person");
}

void Person::addDevice(string type, Agent* agent) {
	m_idDevices.insert(std::pair<string, Agent*>(type, agent));
}

