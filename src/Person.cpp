/*
 * Person.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include "Person.h"
#include <geos/geom/GeometryFactory.h>
using namespace geos;
using namespace geos::geom;


Person::Person() {
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

const Point& Person::getPosition() const {
	return m_position;
}

void Person::setPosition(const Point& position) {
	GeometryFactory::Ptr factory = getWorld().getMap().getGlobalFactory();
	Coordinate c = position.getCoordinate();
	factory->createPoint(c);
}
