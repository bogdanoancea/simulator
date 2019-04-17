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
#include <iomanip>
#include <sstream>

HoldableAgent::HoldableAgent(Map* m, long id, Point* initPosition,
		Agent* holder) :
		MovableAgent(m, id, initPosition), m_holder { holder } {
	// TODO Auto-generated constructor stub

}

HoldableAgent::~HoldableAgent() {
	// TODO Auto-generated destructor stub
}

Agent* HoldableAgent::getHolder() const {
	return m_holder;
}

void HoldableAgent::setHolder(Agent* holder) {
	m_holder = holder;
	LocatableAgent* l = dynamic_cast<LocatableAgent*>(holder);
	if (l != nullptr)
		setLocation(l->getLocation());
}

string HoldableAgent::toString() {
	ostringstream result;
	if (m_holder != nullptr)
		result << MovableAgent::toString() << left << setw(15) << m_holder->getId();
	else
		result << MovableAgent::toString() << left << setw(15) << "null";
	return (result.str());
}
