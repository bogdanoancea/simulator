/*
 * CellIDEventConfig.cpp
 *
 *  Created on: Oct 21, 2020
 *      Author: bogdan
 */

#include <events/CellIDEventConfig.h>

CellIDEventConfig::CellIDEventConfig(unsigned long t, unsigned long cellID, EventCode code, unsigned long deviceID, NetworkType netType): m_timestamp{t},
m_cellID{cellID}, m_code{code}, m_deviceID{deviceID}, m_networkType{netType} {
	// TODO Auto-generated constructor stub

}

const unsigned long CellIDEventConfig::getCellID() {
	return m_cellID;
}

const unsigned long CellIDEventConfig::getDeviceID() {
	return m_deviceID;
}

const unsigned long CellIDEventConfig::getTimestamp() {
	return m_timestamp;
}

const EventCode CellIDEventConfig::getEventCode()  {
	return m_code;
}

const NetworkType CellIDEventConfig::getNetworkType() {
	return m_networkType;
}

CellIDEventConfig::~CellIDEventConfig() {
	// TODO Auto-generated destructor stub
}

