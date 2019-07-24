/*
 * Person.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
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

Person::Person(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed, int age, Gender gen, unsigned long timeStay, unsigned long intervalBetweenStays) :
		MovableAgent(m, id, initPosition, clock, initSpeed), m_age { age }, m_gender { gen } , m_changeDirection {false}, m_timeStay{timeStay},
		m_intervalBetweenStays{intervalBetweenStays} {
			this->m_copyTimeStay = timeStay;
			this->m_copyIntervalBetweenStays = intervalBetweenStays;
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
	ss << MovableAgent::toString() << left << setw(15) << m_age << setw(15) << (m_gender == Person::Gender::MALE? "Male" : "Female");

	if (m_idDevices.size() > 0) {
		for (pair<string, Agent*> i : m_idDevices) {
			HoldableAgent* h = dynamic_cast<HoldableAgent*>(i.second);
			ss << setw(15) << (h->getId());
		}
	}
	return (ss.str());
}

Point* Person::move(MovementType mvType) {
	if (mvType == MovementType::RANDOM_WALK_CLOSED_MAP)
		randomWalkClosedMap();
	else if (mvType == MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT)
		randomWalkClosedMapDrift();
	else
		throw runtime_error("Movement type not yet implemented");

	//get devices and try to connect
//	if (hasDevices()) {
//		unordered_multimap<string, Agent*>::iterator it;
//		for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
//			Agent* a = it->second;
//			HoldableAgent* device = dynamic_cast<HoldableAgent*>(a);
//			if (device != nullptr) {
//				device->tryConnect(HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY);
//			}
//		}
//	}
	return (getLocation());
}

void Person::randomWalkClosedMap() {
	double theta = 0.0;
	theta = RandomNumberGenerator::instance(0)->generateUniformDouble(0.0, 2 * utils::PI);
	Point* pt = generateNewLocation(theta);
	setNewLocation(pt, false);
}

void Person::randomWalkClosedMapDrift() {
	double theta = 0.0;
	double trendAngle = 3 * utils::PI/4;
	if( getClock()->getCurrentTime() >= getClock()->getFinalTime() / 2) {
		trendAngle = 5 * utils::PI/4;
	}
	theta = RandomNumberGenerator::instance(0)->generateNormalDouble(trendAngle, 0.1);
	if(m_changeDirection) {
		theta =  theta + utils::PI/RandomNumberGenerator::instance(0)->generateUniformDouble(0.5, 1.5);
	}
	Point* pt = generateNewLocation(theta);
	setNewLocation(pt, true);
}


Point* Person::generateNewLocation(double theta) {
	double x = getLocation()->getCoordinate()->x;
	double y = getLocation()->getCoordinate()->y;
	double newX = x + getSpeed() * cos(theta);
	double newY = y + getSpeed() * sin(theta);
	Coordinate c = Coordinate(newX, newY, 0);
	Point* pt = getMap()->getGlobalFactory()->createPoint(c);
	return pt;
}

unsigned long Person::getTimeStay() const {
	return m_timeStay;
}

unsigned long Person::getIntervalBetweenStays() const {
	return m_intervalBetweenStays;
}

void Person::setNewLocation(Point* p, bool changeDirection) {
	Geometry* g = getMap()->getBoundary();
	if (p->within(g)) {
		this->getMap()->getGlobalFactory()->destroyGeometry(getLocation());
		setLocation(p);
	}
	else {
		if(changeDirection)
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
	return m_gender;
}

const string Person::getName() const {
	return ("Person");
}

bool Person::stay(unsigned long time_increment) {
	m_intervalBetweenStays-= time_increment;
	if(m_intervalBetweenStays <= 0) {
		m_intervalBetweenStays = (unsigned long)RandomNumberGenerator::instance()->generateExponentialDouble(1.0/m_copyIntervalBetweenStays);
		if(m_intervalBetweenStays < time_increment)
			m_intervalBetweenStays = time_increment;
		return true;
	}
	else {
		return false;
	}
}
