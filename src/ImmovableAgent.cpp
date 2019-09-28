/*
 * Data simulator for mobile phone network events
 *
 * ImmovableAgent.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <ImmovableAgent.h>
#include <iostream>

using namespace geos;
using namespace geos::geom;

ImmovableAgent::ImmovableAgent(const Map* m, const unsigned long id, Point* initialPosition, const Clock* clock) :
		LocatableAgent(m, id, initialPosition, clock) {
}

ImmovableAgent::~ImmovableAgent() {
}

const string ImmovableAgent::toString() const {
	return (LocatableAgent::toString());
}

const string ImmovableAgent::getName() const {
	return ("ImovableAgent");
}
