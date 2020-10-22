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
#include<agent/HoldableAgent.h>

class EventConfig {
public:
	EventConfig();
	virtual const unsigned long getCellID() = 0;
	virtual const unsigned long getDeviceID() = 0;
	virtual const unsigned long getTimestamp() = 0;
	virtual const EventCode getEventCode() = 0;
	virtual const NetworkType getNetworkType() = 0;
	virtual ~EventConfig();
};

#endif /* INCLUDE_EVENTS_EVENTCONFIG_H_ */
