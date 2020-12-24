/*
 * EventConfig.h
 *
 *  Created on: Oct 21, 2020
 *      Author: bogdan
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
