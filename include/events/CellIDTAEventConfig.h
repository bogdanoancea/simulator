/*
 * CellIDTAEventConfig.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_EVENTS_CELLIDTAEVENTCONFIG_H_
#define INCLUDE_EVENTS_CELLIDTAEVENTCONFIG_H_

#include <events/CellIDEventConfig.h>

class CellIDTAEventConfig: public CellIDEventConfig {
public:
	CellIDTAEventConfig(unsigned long t, unsigned long cellID, EventCode code, unsigned long deviceID, NetworkType netType, unsigned int ta);
	const unsigned int getTA();
	virtual ~CellIDTAEventConfig();

private:
	unsigned int m_ta;
};

#endif /* INCLUDE_EVENTS_CELLIDTAEVENTCONFIG_H_ */
