/*
 * Data simulator for mobile phone network events
 *
 * Clock.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <Clock.h>
#include <sys/_types/_time_t.h>

using namespace std;
using namespace std::chrono;


Clock::Clock() :
		m_initialTime { 0 }, m_currentTime { 0 }, m_increment { 1 }, m_finalTime { 0 } {
}

Clock::Clock(unsigned int init, unsigned int inc) :
		m_initialTime { init }, m_currentTime { init }, m_increment { inc },m_finalTime{init} {
}

Clock::~Clock() {
}

unsigned int Clock::tick() {
	m_currentTime += m_increment;
	return (m_currentTime);
}

unsigned int Clock::getCurrentTime() const {
	return m_currentTime;
}

void Clock::setCurrentTime(unsigned int currentTime) {
	m_currentTime = currentTime;
}

unsigned int Clock::getIncrement() const {
	return m_increment;
}

void Clock::setIncrement(unsigned int increment) {
	m_increment = increment;
}

unsigned int Clock::getInitialTime() const {
	return m_initialTime;
}

void Clock::setInitialTime(unsigned int initialTime) {
	m_initialTime = initialTime;
}

time_t Clock::realTime() {
	auto t = system_clock::now();
	time_t time = system_clock::to_time_t(t);
	return (time);
}
