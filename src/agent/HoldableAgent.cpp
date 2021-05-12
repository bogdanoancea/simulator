/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * HoldableAgent.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */


#include <agent/HoldableAgent.h>
#include <iomanip>
#include <sstream>
#include <algorithm>

HoldableAgent::HoldableAgent(const Map* m, const unsigned long id, Point* initPosition, Agent* holder, const Clock* clock) :
		MovableAgent(m, id, initPosition, clock, 0.0), m_holder { holder } {
}

HoldableAgent::HoldableAgent(const HoldableAgent &h) :
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
	if (l != nullptr) {
		setLocation(l->getLocation());
	}
	MovableAgent* m = dynamic_cast<MovableAgent*>(holder);
	if (m != nullptr) {
		setSpeed(m->getSpeed());
	} else
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
	tryConnect();
}

const string HoldableAgent::toString(bool detailed) const {
	ostringstream result;
	if (m_holder != nullptr)
		result << MovableAgent::toString() << left << setw(15) << m_holder->getId();
	else
		result << MovableAgent::toString() << left << setw(15) << "null";
	return (result.str());
}

