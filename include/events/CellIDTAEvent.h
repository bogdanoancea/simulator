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
 * CellIDTAEvent.h
 *
 *  Created on: Oct 20, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef CELLIDTAEVENT_H_
#define CELLIDTAEVENT_H_

#include <events/Event.h>
#include <events/CellIDTAEventConfig.h>
#include <string>



class CellIDTAEvent: public Event {
public:
	friend class EventFactory;
	virtual const string toString() override;
	virtual EventConfig* getEventConfig() override;
	virtual ~CellIDTAEvent();
protected:
	CellIDTAEvent(EventConfig* config);
private:
	EventConfig* m_config;
};

#endif /* SRC_EVENTS_CELLIDTAEVENT_H_ */
