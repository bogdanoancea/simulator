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
 * MovableAgent.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/MovableAgent.h>
#include <iomanip>
#include <sstream>

MovableAgent::MovableAgent(const Map* m, const unsigned long id, Point* initialPosition, const Clock* clock, double initialSpeed) :
		LocatableAgent(m, id, initialPosition, clock), m_speed { initialSpeed } {
}

MovableAgent::~MovableAgent() {
}

double MovableAgent::getSpeed() const {
	return (m_speed);
}

void MovableAgent::setSpeed(double val) {
	m_speed = val;
}

const string MovableAgent::toString() const {
	ostringstream ss;
	ss << LocatableAgent::toString() << left << setw(15) << m_speed;
	return (ss.str());
}
