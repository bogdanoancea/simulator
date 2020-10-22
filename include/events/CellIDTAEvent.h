/*
 * CellIDTAEvent.h
 *
 *  Created on: Oct 20, 2020
 *      Author: bogdan
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
