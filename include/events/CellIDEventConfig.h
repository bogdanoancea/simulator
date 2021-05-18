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
 * CellIDEventConfig.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_EVENTS_CELLIDEVENTCONFIG_H_
#define INCLUDE_EVENTS_CELLIDEVENTCONFIG_H_

#include <events/EventConfig.h>
#include <NetworkType.h>

class CellIDEventConfig: public EventConfig {
public:
	CellIDEventConfig(unsigned long t, unsigned long cellID, EventCode code, unsigned long deviceID, NetworkType netType);
	virtual const unsigned long getCellID() override;
	virtual const unsigned long getDeviceID() override;
	virtual const unsigned long getTimestamp() override;
	virtual const EventCode getEventCode() override;
	virtual const NetworkType getNetworkType() override;
	virtual ~CellIDEventConfig();

private:
	unsigned long m_timestamp;
	unsigned long m_cellID;
	EventCode m_code;
	unsigned long m_deviceID;
	NetworkType m_networkType;


};

#endif /* INCLUDE_EVENTS_CELLIDEVENTCONFIG_H_ */
