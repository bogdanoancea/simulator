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

/**
 * This is the factory class that builds different types of network events according to the configuration of the simulator.
 */
class EventFactory {
public:
	/**
	 * Default constructor
	 */
	EventFactory();

	/**
	 * Creates a concrete Event object according to the type of the EventConfig type. Two types of events are implemented: CellIDEvent and CellIDTAEvent.
	 * CellIDEvent contains the IDD of the cell where a mobile device was detected at a time instant, the network type and the timestamp of the event and the CellIDTAEvent additionally
	 * contains the time advancing value of the event.
	 * @return a pointer to a concrete Event object according to the type of the EventConfig type. The two type of event objects supported are CellIDEvent and CellIDTAEvent.
	 */
	Event* createEvent(EventConfig* config);
	virtual ~EventFactory();
};

#endif /* EVENTFACTORY_H_ */


