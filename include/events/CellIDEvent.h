/*
 * CellIDEvent.h
 *
 *  Created on: Oct 20, 2020
 *      Author: bogdan
 */

#ifndef SRC_EVENTS_CELLIDEVENT_H_
#define SRC_EVENTS_CELLIDEVENT_H_

#include <events/Event.h>
#include <events/EventConfig.h>


class CellIDEvent: public Event {
public:
	friend class EventFactory;
	virtual const string toString() override;
	virtual EventConfig* getEventConfig() override;
	virtual ~CellIDEvent();
protected:
	CellIDEvent(EventConfig* conf);
private:
	EventConfig* m_config;
};

#endif /* SRC_EVENTS_CELLIDEVENT_H_ */
