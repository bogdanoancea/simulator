/*
 * Person.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <Agent.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/LineString.h>

#include <HoldableAgent.h>
#include <Map.h>
#include <Person.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <utility>

#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>

using namespace geos;
using namespace geos::geom;

Person::Person(Map* m, long id, Point* initPosition, Clock* clock, double initSpeed, int age) :
		MovableAgent(m, id, initPosition, clock, initSpeed), m_age { age } {
	// TODO Auto-generated constructor stub

}

Person::~Person() {
	// TODO Auto-generated destructor stub
}

int Person::getAge() const {
	return (m_age);
}

void Person::setAge(int age) {
	m_age = age;
}

string Person::toString() {
	ostringstream ss;
	ss << MovableAgent::toString() << left << setw(15) << m_age;

	if (m_idDevices.size() > 0) {
		for (pair<string, Agent*> i : m_idDevices) {
			HoldableAgent* h = dynamic_cast<HoldableAgent*>(i.second);
			ss << setw(15) << (h->getId());
		}
	}
	return (ss.str());
}

Point* Person::move() {
	double theta = 0.0;
	double x, y, newX, newY;
	theta = RandomNumberGenerator::instance()->generateDouble(0.0, 2 * utils::PI);
	x = getLocation()->getCoordinate()->x;
	y = getLocation()->getCoordinate()->y;
	newX = x + getSpeed() * cos(theta);
	newY = y + getSpeed() * sin(theta);
	Geometry* g = getMap()->getBoundary();
	if (dynamic_cast<Polygon*>(g) != nullptr) {
		Polygon* p = dynamic_cast<Polygon*>(g);
		const Envelope* env = p->getEnvelopeInternal();
		//double xmin = env->getMinX();
		//double xmax = env->getMaxX();
		//double ymin = env->getMinY();
		//double ymax = env->getMaxX();
		Coordinate c = Coordinate(newX, newY);
		Point* pt = getMap()->getGlobalFactory()->createPoint(c);

		if (pt->within(g)) {
			this->getMap()->getGlobalFactory()->destroyGeometry(getLocation());
			setLocation(pt);
		} else {
			cout << "ies afara" << endl;
			CoordinateSequence* cl = new CoordinateArraySequence();
			cl->add(Coordinate(x, y));
			cl->add(Coordinate(newX, newY));

			LineString* ls = this->getMap()->getGlobalFactory()->createLineString(cl);
			Geometry* intersect = ls->intersection(g);
			Point* ptInt = dynamic_cast<Point*>(intersect);
			this->getMap()->getGlobalFactory()->destroyGeometry(getLocation());
			setLocation(ptInt);
			// fac o linie (x,y) la (newX, newY)
			// intersectez linia asta cu g si iau primul punct de intersectie
			// asta va fi noua locatie

		}

		/*
		 if (newX > xmax)
		 newX = xmax;
		 if (newX < xmin)
		 newX = xmin;
		 if (newY > ymax)
		 newY = ymax;
		 if (newY < ymin)
		 newY = ymin;

		 Coordinate c = Coordinate(newX, newY);
		 Point* pt = getMap()->getGlobalFactory()->createPoint(c);

		 this->getMap()->getGlobalFactory()->destroyGeometry(getLocation());

		 setLocation(pt);
		 */
		//get devices and set the location for them
		if (hasDevices()) {
			unordered_multimap<string, Agent*>::iterator it;
			for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
				Agent* a = it->second;
				HoldableAgent* device = dynamic_cast<HoldableAgent*>(a);
				if (device != nullptr) {
					device->tryConnect();
				}
			}
		}
	}
	return (getLocation());
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
	char sep = ',';
	unordered_multimap<string, Agent*>::iterator it;
	for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
		Agent* a = it->second;
		ss << sep << a->getId();
	}
	return (ss.str());
}
