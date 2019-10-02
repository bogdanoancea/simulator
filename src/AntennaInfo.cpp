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

using namespace std;

AntennaInfo::AntennaInfo(const unsigned long time, const unsigned long antennaId, const unsigned long event, const unsigned long deviceId, const double x, const double y) :
		m_time { time }, m_antennaId { antennaId }, m_eventCode { event }, m_deviceId { deviceId }, m_x { x }, m_y { y } {
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

const string AntennaInfo::toString() const {
	ostringstream result;
	const char sep = Constants::sep;
	result << m_time << sep << m_antennaId << sep << m_eventCode << sep << m_deviceId << sep << fixed << m_x << sep << m_y;
	return (result.str());
}

bool AntennaInfo::operator <(const AntennaInfo& ai) const {
	return (m_time < ai.getTime());
}
