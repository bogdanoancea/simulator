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

AntennaInfo::AntennaInfo(unsigned long time, unsigned long antennaId, unsigned long event, unsigned long deviceId, double x, double y) :
		m_time { time }, m_antennaId { antennaId }, m_eventCode { event }, m_deviceId { deviceId }, m_x { x }, m_y { y } {
	// TODO Auto-generated constructor stub

}

unsigned long AntennaInfo::getAntennaId() const {
	return (m_antennaId);
}

unsigned long AntennaInfo::getDeviceId() const {
	return m_deviceId;
}

unsigned long AntennaInfo::getEventCode() const {
	return m_eventCode;
}

unsigned long AntennaInfo::getTime() const {
	return m_time;
}

double AntennaInfo::getX() const {
	return m_x;
}

double AntennaInfo::getY() const {
	return m_y;
}

string AntennaInfo::toString() const {
	ostringstream result;
	result << m_time << "," << m_antennaId << "," << m_eventCode << "," << m_deviceId << "," << m_x << "," << m_y;
	return (result.str());
}
