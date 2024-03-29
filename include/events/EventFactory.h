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
 * EventFactory.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
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
	 * Creates a concrete Event object according to the actual type of the <tt>config</tt> argument.
	 * Two types of events are implemented: CellIDEvent and CellIDTAEvent.
	 * CellIDEvent contains the ID of the cell where a mobile device was detected at a time instant, the network type and the timestamp of the event
	 * and the CellIDTAEvent additionally contains the timing advance value of the event.
	 * @return a pointer to a concrete Event object created according to the actual type of the of the <tt>config</tt> argument.
	 * The two types of event objects supported are:
	 * \li CellIDEvent and in this case the <tt>config</tt> parameter should point to a CellIDEventconfig object
	 * \li CellIDTAEvent and in this case the <tt>config</tt> parameter should point to a CellIDTAEventconfig object.
	 */
	Event* createEvent(EventConfig* config);

	/**
	 * Default destructor.
	 */
	virtual ~EventFactory();
};

#endif /* EVENTFACTORY_H_ */


