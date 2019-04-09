/*
 * Data simulator for mobile phone network events
 *
 * HoldableAgent.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include "HoldableAgent.h"
#include "LocatableAgent.h"

HoldableAgent::HoldableAgent(World& w, Point& initPosition, long idHolder) :
		MovableAgent(w, initPosition), m_idHolder { idHolder } {
	// TODO Auto-generated constructor stub

}

HoldableAgent::~HoldableAgent() {
	// TODO Auto-generated destructor stub
}

long HoldableAgent::getIdHolder() const {
	return m_idHolder;
}

void HoldableAgent::setIdHolder(long idHolder) {
	m_idHolder = idHolder;
}
