/*
 * Data simulator for mobile phone network events
 *
 * HoldableAgent.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <Agent.h>
#include <HoldableAgent.h>
#include <iomanip>
#include <sstream>

HoldableAgent::HoldableAgent(Map* m, long id, Point* initPosition, Agent* holder) :
		MovableAgent(m, id, initPosition, 0.0), m_holder { holder } , m_isConnected {false}{
	// TODO Auto-generated constructor stub

}

HoldableAgent::HoldableAgent(const HoldableAgent &h):
		MovableAgent(h.getMap(), h.getId(), h.getLocation(), 0.0) {
	m_holder = h.getHolder();
	m_isConnected = h.isConnected();
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

	MovableAgent* m = dynamic_cast<MovableAgent*>(holder);
	if (m != nullptr)
		setSpeed(m->getSpeed());
	else
		setSpeed(0.0);
}

bool HoldableAgent::isConnected() const {
	return m_isConnected;
}

void HoldableAgent::setConnected(bool c) {
	m_isConnected = c;
}

string HoldableAgent::toString() {
	ostringstream result;
	if (m_holder != nullptr)
		result << MovableAgent::toString() << left << setw(15) << m_holder->getId();
	else
		result << MovableAgent::toString() << left << setw(15) << "null";
	return (result.str());
}
