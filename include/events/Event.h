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
/**
 * This is the interface of an event object. All types of events should implement the functions declared here.
 * An Event object is to be created by the EventFactory.
 */
class Event {
public:
	/**
	 * @return The string representation of an event. The content of the string depends on the conctre type of the event.
	 */
	virtual const string toString() = 0;
	/**
	 * return A pointer to the EventConfig object used to create the event by the factory method.
	 */
	virtual EventConfig* getEventConfig() = 0;

	/**
	 * Default destructor.
	 */
	virtual ~Event();
protected:
	Event();
};

#endif /* EVENT_H_ */
