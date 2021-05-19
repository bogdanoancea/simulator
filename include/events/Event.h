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
 * Event.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
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
	 * Returns the string representation of an Event object.
	 * @return the string representation of an Event object. The content of the string depends on the concrete type of the event.
	 */
	virtual const string toString() = 0;

	/**
	 * Returns a pointer to the EventConfig object used to create the Event object by the factory method.
	 * return a pointer to the EventConfig object used to create the Event object by the factory method.
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
