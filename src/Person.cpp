/*
 * Person.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include "Person.h"
#include "MovableAgent.h"
#include <geos/geom/GeometryFactory.h>
#include <iomanip>
#include <sstream>


using namespace geos;
using namespace geos::geom;


Person::Person(Map* m, long id, Point* initPosition, int age) :
		MovableAgent(m, id, initPosition), m_age { age } {
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
		for (pair<string, long> i : m_idDevices) {
			ss << setw(15) << (i.second);
		}
	}
	return (ss.str());
}
