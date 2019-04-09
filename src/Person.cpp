/*
 * Person.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include "Person.h"
#include "MovableAgent.h"
#include <geos/geom/GeometryFactory.h>
using namespace geos;
using namespace geos::geom;


Person::Person(World& w, Point& initPosition, long id, int age) :
		MovableAgent(w, initPosition), m_id { id }, m_age { age } {
	// TODO Auto-generated constructor stub

}

Person::~Person() {
	// TODO Auto-generated destructor stub
}

int Person::getAge() const {
	return m_age;
}

void Person::setAge(int age) {
	m_age = age;
}

long Person::getId() const {
	return m_id;
}

void Person::setId(long id) {
	m_id = id;
}

