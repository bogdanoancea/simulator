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
 * Clock.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <Clock.h>
#include <ctime>
#include <Constants.h>

using namespace std;
using namespace std::chrono;


Clock::Clock(unsigned long init, unsigned long end, unsigned long inc) :
		m_initialTime { init }, m_currentTime { init }, m_increment { inc }, m_finalTime { end } {
}

Clock::~Clock() {
}

unsigned long Clock::tick() {
	m_currentTime += m_increment;
	return (m_currentTime);
}

void Clock::reset() {
	m_currentTime = m_initialTime;
}

unsigned long Clock::getCurrentTime() const {
	return (m_currentTime);
}

void Clock::setCurrentTime(unsigned long currentTime) {
	m_currentTime = currentTime;
}

unsigned long Clock::getIncrement() const {
	return (m_increment);
}

void Clock::setIncrement(unsigned long increment) {
	m_increment = increment;
}

unsigned long Clock::getInitialTime() const {
	return (m_initialTime);
}

void Clock::setInitialTime(unsigned long initialTime) {
	m_initialTime = initialTime;
}

time_t Clock::realTime() {
	auto t = system_clock::now();
	time_t time = system_clock::to_time_t(t);
	return (time);
}

unsigned long Clock::getFinalTime() const {
	return (m_finalTime);
}

void Clock::setFinalTime(unsigned long finalTime) {
	m_finalTime = finalTime;
}
