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
#include <algorithm>

HoldableAgent::HoldableAgent(Map* m, long id, Point* initPosition, Agent* holder, Clock* clock) :
		MovableAgent(m, id, initPosition, clock, 0.0), m_holder { holder } {
}

HoldableAgent::HoldableAgent(const HoldableAgent &h):
	MovableAgent(h.getMap(), h.getId(), h.getLocation(), h.getClock(), 0.0) {
	m_holder = h.getHolder();
	std::copy(h.getAntennas().begin(), h.getAntennas().end(), m_antennas.begin());
}

HoldableAgent::~HoldableAgent() {
}

Agent* HoldableAgent::getHolder() const {
	return (m_holder);
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
	return (m_antennas.size() > 0);
}

vector<Antenna*> HoldableAgent::getAntennas() const {
	return (m_antennas);
}

void HoldableAgent::setLocation(Point* location) {
	LocatableAgent::setLocation(location);
	tryConnect(HoldableAgent::CONNECTION_TYPE::UNKNOWN);
}

string HoldableAgent::toString() const{
	ostringstream result;
	if (m_holder != nullptr)
		result << MovableAgent::toString() << left << setw(15) << m_holder->getId();
	else
		result << MovableAgent::toString() << left << setw(15) << "null";
	return (result.str());
}
