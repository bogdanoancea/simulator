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

/**
 * This class encapsulated the parameters needed by the factory class to build a CellIDEvent object.
 */
class CellIDEventConfig: public EventConfig {
public:
	/**
	 * Constructor of the class. Builds a CellIDEventConfig object.
	 * @param t the timestamp of the event. This is the time measured by the simulation clock.
	 * @param cellID the id of the Antenna object which generated the event.
	 * @param code - the event code. It can take one of the following values:
	 * \li EventCode::ATTACH_DEVICE
	 * \li EventCode::DETACH_DEVICE
	 * \li EventCode::ALREADY_ATTACHED_DEVICE
	 * \li EventCode::IN_RANGE_NOT_ATTACHED_DEVICE
	 * @param deviceID the id of the mobile device which generated the event.
	 * @param netType the network type (a parameter of the antenna). It can take one of the following values:
	 * \li NetworkType::_3G
	 * \li NetworkType::_4G
	 */
	CellIDEventConfig(unsigned long t, unsigned long cellID, EventCode code, unsigned long deviceID, NetworkType netType);

	/**
	 * Returns the id of the Antenna object which generated the event.
	 * @return the id of the Antenna object which generated the event.
	 */
	virtual const unsigned long getCellID() override;

	/**
	 * Returns the id of the mobile device which generated the event.
	 * @return the id of the mobile device which generated the event.
	 */
	virtual const unsigned long getDeviceID() override;

	/**
	 * Returns the timestamp of the event (measured by the simulation clock).
	 * @return the timestamp of the event (measured by the simulation clock).
	 */
	virtual const unsigned long getTimestamp() override;

	/**
	 * Returns the event code.
	 * @return the event code. It can take one of the following values:
	 * \li EventCode::ATTACH_DEVICE
	 * \li EventCode::DETACH_DEVICE
	 * \li EventCode::ALREADY_ATTACHED_DEVICE
	 * \li EventCode::IN_RANGE_NOT_ATTACHED_DEVICE
	 */
	virtual const EventCode getEventCode() override;

	/**
	 * Returns the network type of the Antenna object which generated this event.
	 * @return the network type of the Antenna object which generated this event. It can take one of the following values:
	 * \li NetworkType::_3G
	 * \li NetworkType::_4G
	 */
	virtual const NetworkType getNetworkType() override;

	/**
	 * Default destructor.
	 */
	virtual ~CellIDEventConfig();

private:
	unsigned long m_timestamp;
	unsigned long m_cellID;
	EventCode m_code;
	unsigned long m_deviceID;
	NetworkType m_networkType;


};

#endif /* INCLUDE_EVENTS_CELLIDEVENTCONFIG_H_ */
