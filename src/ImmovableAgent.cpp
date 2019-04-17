/*
 * Data simulator for mobile phone network events
 *
 * ImmovableAgent.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include "ImmovableAgent.h"
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;


ImmovableAgent::ImmovableAgent(Map* m, long id, Point* initialPosition) :
		LocatableAgent(m, id, initialPosition) {
	// TODO Auto-generated constructor stub
}

ImmovableAgent::~ImmovableAgent() {
	// TODO Auto-generated destructor stub
}

string ImmovableAgent::toString() {
	return (LocatableAgent::toString());
}
