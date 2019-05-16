/*
 * Data simulator for mobile phone network events
 *
 * AntennaInfo.cpp
 *
 *  Created on: May 16, 2019
 *      Author: Bogdan Oancea
 */

#include <AntennaInfo.h>

AntennaInfo::AntennaInfo(unsigned long time, unsigned long antennaId, unsigned long event, unsigned long deviceId, double x, double y) :
		m_time { time }, m_antennaId { antennaId }, m_eventCode { event }, m_deviceId { deviceId }, m_x { x }, m_y { y } {
	// TODO Auto-generated constructor stub

}

