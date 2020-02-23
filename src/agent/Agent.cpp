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
 * Agent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/Agent.h>

Agent::Agent() : m_id{0} {
	m_map = nullptr;
	m_clock = nullptr;
}

Agent::Agent(const Map* m, const unsigned long id, const Clock* clock) :
		m_id { id } {
	m_map = m;
	m_clock = clock;
}

Agent::~Agent() {
}

bool Agent::operator==(const Agent& a) {
	return (this->m_id == a.getId());
}

const Map* Agent::getMap() const {
	return (m_map);
}


const Clock* Agent::getClock() const {
	return (m_clock);
}

const unsigned long Agent::getId() const {
	return (m_id);
}
