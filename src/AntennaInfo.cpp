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
 * AntennaInfo.cpp
 *
 *  Created on: May 16, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <AntennaInfo.h>
#include <string>
#include <sstream>
#include <Constants.h>
#include <events/EventType.h>
#include <iostream>


using namespace std;

AntennaInfo::AntennaInfo(EventType evtType, Row r): m_eventType{evtType}  {
	m_time = stoul(r[0]);
	unsigned int size = r.size();
	for(unsigned int i = 0; i < size; i++)
		m_textRow.push_back(r[i]);

}


//AntennaInfo::AntennaInfo(const unsigned long time, const unsigned long antennaId, const unsigned long event, const unsigned long deviceId, const double x, const double y) :
//		m_time { time }, m_antennaId { antennaId }, m_eventCode { event }, m_deviceId { deviceId }, m_x { x }, m_y { y } {
//}

const unsigned long AntennaInfo::getAntennaId() const {
	unsigned long result =-1;
	if(m_eventType == EventType::CELLID || m_eventType == EventType::CELLIDTA)
		result = stoul(m_textRow[1]);
	return result;
}

const unsigned long AntennaInfo::getDeviceId() const {
	unsigned long result =-1;
	if(m_eventType == EventType::CELLID || m_eventType == EventType::CELLIDTA)
		result = stoul(m_textRow[3]);

	return result;

}

const unsigned long AntennaInfo::getEventCode() const {
	unsigned long result =-1;
	if(m_eventType == EventType::CELLID || m_eventType == EventType::CELLIDTA)
		result = stoul(m_textRow[2]);
	return result;
}

unsigned long AntennaInfo::getTime() const {
	return (m_time);
}

double AntennaInfo::getX() const {
	double result = -1;
	if(m_eventType == EventType::CELLID || m_eventType == EventType::CELLIDTA)
		result = stod(m_textRow[5]);
	return result;
}

double AntennaInfo::getY() const {
	double result = -1;
	if(m_eventType == EventType::CELLID || m_eventType == EventType::CELLIDTA)
		result = stod(m_textRow[6]);
	return result;
}

const string AntennaInfo::toString() const {
	ostringstream result;
	const char sep = Constants::sep;
	for( unsigned int i = 0; i < m_textRow.size()-1; i++)
		result << m_textRow[i] << sep;

	result << m_textRow[m_textRow.size()-1];

	return (result.str());
}

bool AntennaInfo::operator <(const AntennaInfo& ai) const {
	return (m_time < ai.getTime());
}

