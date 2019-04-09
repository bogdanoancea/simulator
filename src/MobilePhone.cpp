/*
 * Data simulator for mobile phone network events
 *
 * MobilePhone.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include "MobilePhone.h"
#include "HoldableAgent.h"


MobilePhone::MobilePhone(World& w, Point& initPosition, long phoneId) :
		HoldableAgent(w, initPosition, -1), m_phoneId { phoneId } {
	// TODO Auto-generated constructor stub

}

MobilePhone::~MobilePhone() {
	// TODO Auto-generated destructor stub
}


long MobilePhone::getPhoneId() const {
	return m_phoneId;
}

void MobilePhone::setPhoneId(long phoneId) {
	m_phoneId = phoneId;
}
