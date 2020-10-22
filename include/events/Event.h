/*
 * Event.h
 *
 *  Created on: Oct 20, 2020
 *      Author: bogdan
 */

#ifndef EVENT_H_
#define EVENT_H_

#include <events/EventCode.h>
#include <events/EventConfig.h>
#include <NetworkType.h>
#include <string>

using namespace std;

class Event {
public:
	virtual const string toString() = 0;
	virtual EventConfig* getEventConfig() = 0;
	virtual ~Event();
protected:
	Event();
};

#endif /* EVENT_H_ */
