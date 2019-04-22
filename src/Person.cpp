/*
 * Person.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <Agent.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>
#include <HoldableAgent.h>
#include <Map.h>
#include <Person.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <cmath>
#include <iomanip>
#include <random>
#include <sstream>
#include <utility>

using namespace geos;
using namespace geos::geom;



Person::Person(Map* m, long id, Point* initPosition, double initSpeed, int age) :
		MovableAgent(m, id, initPosition, initSpeed), m_age { age } {
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

Point& Person::move(std::mt19937& generator) {
	double theta = 0.0;
	double newX, newY;
	//uniform_real_distribution<double>::param_type p(0.0, 2 * utils::PI);
	//uniform_real_distribution<double> r = RandomNumberGenerator::instance()->getUnifDoubleDistribution();
	//r.param(p);
	theta = RandomNumberGenerator::instance()->generateDouble(0.0, 2 * utils::PI);

	newX = getLocation().getCoordinate()->x + getSpeed() * cos(theta);
	newY = getLocation().getCoordinate()->y + getSpeed() * sin(theta);
	Geometry* g = getMap()->getBoundary();
	if (dynamic_cast<Polygon*>(g) != nullptr) {
		Polygon* p = dynamic_cast<Polygon*>(g);
		const Envelope* env = p->getEnvelopeInternal();
		double xmin = env->getMinX();
		double xmax = env->getMaxX();
		double ymin = env->getMinY();
		double ymax = env->getMaxX();
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
		setLocation(*pt);
		//get devices and set the location for them
		int d = m_idDevices.size();
		if (d > 0) {
			unordered_multimap<string, Agent*>::iterator it;
			for (it = m_idDevices.begin(); it != m_idDevices.end(); it++) {
				Agent* a = it->second;
				LocatableAgent* l = dynamic_cast<LocatableAgent*>(a);
				if (l != nullptr)
					l->setLocation(*pt);
			}
		}
	}
	return (getLocation());
}

bool Person::hasDevices() {
	return (m_idDevices.size() > 0);
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
