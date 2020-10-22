/*
 * EventFactory.h
 *
 *  Created on: Oct 20, 2020
 *      Author: bogdan
 */

#ifndef EVENTFACTORY_H_
#define EVENTFACTORY_H_

#include <events/EventConfig.h>
#include <events/Event.h>


class EventFactory {
public:
	EventFactory();
	Event* createEvent(EventConfig* config);
	virtual ~EventFactory();
};

#endif /* EVENTFACTORY_H_ */


