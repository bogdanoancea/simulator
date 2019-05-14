/*
 * Data simulator for mobile phone network events
 *
 * Tablet.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <Tablet.h>
#include <iostream>

Tablet::Tablet(Map* m, long id, Point* initPosition, Clock* clock) :
		HoldableAgent(m, id, initPosition, nullptr, clock) {
// TODO Auto-generated constructor stub

}

Tablet::~Tablet() {
	// TODO Auto-generated destructor stub
}

string Tablet::toString() {
	return (HoldableAgent::toString());
}

bool Tablet::tryConnect(HoldableAgent::CONNECTION_TYPE type) {
	return false;
}
