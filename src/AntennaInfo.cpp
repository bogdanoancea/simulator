/*
 * Data simulator for mobile phone network events
 *
 * AntennaInfo.cpp
 *
 *  Created on: May 16, 2019
 *      Author: Bogdan Oancea
 */

#include <AntennaInfo.h>
#include <string>
#include <sstream>

using namespace std;

AntennaInfo::AntennaInfo(const unsigned long time, const unsigned long antennaId, const unsigned long event, const unsigned long deviceId, const double x, const double y) :
m_time {time}, m_antennaId {antennaId}, m_eventCode {event}, m_deviceId {deviceId}, m_x {x}, m_y {y} {
}

const unsigned long AntennaInfo::getAntennaId() const {
	return (m_antennaId);
}

const unsigned long AntennaInfo::getDeviceId() const {
	return m_deviceId;
}

const unsigned long AntennaInfo::getEventCode() const {
	return m_eventCode;
}

const unsigned long AntennaInfo::getTime() const {
	return m_time;
}

const double AntennaInfo::getX() const {
	return m_x;
}

const double AntennaInfo::getY() const {
	return m_y;
}

const string AntennaInfo::toString() const {
	ostringstream result;
	result << m_time << "," << m_antennaId << "," << m_eventCode << ","
			<< m_deviceId << "," << m_x << "," << m_y;
	return (result.str());
}
