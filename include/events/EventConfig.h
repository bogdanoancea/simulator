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
 * EventConfig.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef EVENTCONFIG_H_
#define EVENTCONFIG_H_

#include <events/EventCode.h>
#include <NetworkType.h>
#include <agent/HoldableAgent.h>

/**
 * This is the base class of the EventConfig objects. It defines the interface the all derived classes should implement. An EventConfig object
 * is used by the EventFactory to build a specific Event object.
 */
class EventConfig {
public:
	/**
	 * Default constructor
	 */
	EventConfig();
	/**
	 * This is a  pure virtual method that should be implemented by all derived classes. It returns the ID of the cell where the event was recorded by the network.
	 * @return Returns the ID of the cell where the event was recorded by the network.
	 */
	virtual const unsigned long getCellID() = 0;

	/**
	 * This is a  pure virtual method that should be implemented by all derived classes. It returns the ID of the device that generated the event recorded by the network.
	 * @return Returns the ID of the device that generated the event recorded by the network.
	 */
	virtual const unsigned long getDeviceID() = 0;

	/**
	 * This is a  pure virtual method that should be implemented by all derived classes. It returns the time when the event was recorded by the network.
	 */
	virtual const unsigned long getTimestamp() = 0;

	/**
	 * This is a  pure virtual method that should be implemented by all derived classes. It returns the type of the event recorded by the network.
	 * @ return Returns oneof the following values defined in the EventCode class: ATTACH_DEVICE, DETACH_DEVICE, ALREADY_ATTACHED_DEVICE, IN_RANGE_NOT_ATTACHED_DEVICE
	 */
	virtual const EventCode getEventCode() = 0;

	/**
	 *  This is a  pure virtual method that should be implemented by all derived classes. It returns the type the network.
	 *  @return Retunrs the type of the network: _3G, _4G. These values are defined in NetworkType class.
	 */
	virtual const NetworkType getNetworkType() = 0;
	/**
	 * Default destructor
	 */
	virtual ~EventConfig();
};

#endif /* INCLUDE_EVENTS_EVENTCONFIG_H_ */
