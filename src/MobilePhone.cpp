/*
 * Data simulator for mobile phone network events
 *
 * MobilePhone.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <MobilePhone.h>


MobilePhone::MobilePhone(Map* m, long id, Point* initPosition, Agent* holder) :
		HoldableAgent(m, id, initPosition, nullptr) {
	// TODO Auto-generated constructor stub

}

MobilePhone::~MobilePhone() {
	// TODO Auto-generated destructor stub
}

string MobilePhone::toString() {
	return (HoldableAgent::toString());
}
