/*
 * CellIDEventConfig.h
 *
 *  Created on: Oct 21, 2020
 *      Author: bogdan
 */

#ifndef INCLUDE_EVENTS_CELLIDEVENTCONFIG_H_
#define INCLUDE_EVENTS_CELLIDEVENTCONFIG_H_

#include <events/EventConfig.h>
#include <NetworkType.h>

class CellIDEventConfig: public EventConfig {
public:
	CellIDEventConfig(unsigned long t, unsigned long cellID, EventCode code, unsigned long deviceID, NetworkType netType);
	virtual const unsigned long getCellID() override;
	virtual const unsigned long getDeviceID() override;
	virtual const unsigned long getTimestamp() override;
	virtual const EventCode getEventCode() override;
	virtual const NetworkType getNetworkType() override;
	virtual ~CellIDEventConfig();

private:
	unsigned long m_timestamp;
	unsigned long m_cellID;
	EventCode m_code;
	unsigned long m_deviceID;
	NetworkType m_networkType;


};

#endif /* INCLUDE_EVENTS_CELLIDEVENTCONFIG_H_ */
