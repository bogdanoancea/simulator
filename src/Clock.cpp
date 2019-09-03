/*
 * Data simulator for mobile phone network events
 *
 * Clock.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <Clock.h>
#include <ctime>
#include <Constants.h>

using namespace std;
using namespace std::chrono;

Clock::Clock() :
		m_initialTime { Constants::START_TIME }, m_currentTime { Constants::START_TIME }, m_increment { Constants::INCREMENT_TIME }, m_finalTime { Constants::END_TIME } {
}

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
