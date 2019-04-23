/*
 * Data simulator for mobile phone network events
 *
 * MobilePhone.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <MobilePhone.h>
#include <EMField.h>
#include <iostream>
#include <utility>

using namespace std;

MobilePhone::MobilePhone(Map* m, long id, Point* initPosition, Agent* holder, double powerThreshold) :
		HoldableAgent(m, id, initPosition, nullptr), m_powerThreshold { powerThreshold } {
	// TODO Auto-generated constructor stub

}

MobilePhone::~MobilePhone() {
	// TODO Auto-generated destructor stub
}

string MobilePhone::toString() {
	return (HoldableAgent::toString());
}

bool MobilePhone::tryConnect() {
	return tryConnectNaiveAlgorithm();
}

bool MobilePhone::tryConnectNaiveAlgorithm() {


	//select the most powerful antenna
	Point *p = this->getLocation();
	pair<Antenna*, double> antenna = EMField::instance()->computeMaxPower(p);
	if (antenna.second > m_powerThreshold) {
		antenna.first->tryConnect(this);
	}

	// connect to it if power > powerThreshold and maxConnection is not reached
	// other wise try another antenna

	//if successful record the antenna id and return true

	//otherwise return false
	return (false);
}
