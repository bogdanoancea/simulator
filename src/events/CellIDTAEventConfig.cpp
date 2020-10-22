/*
 * CellIDTAEventConfig.cpp
 *
 *  Created on: Oct 21, 2020
 *      Author: bogdan
 */

#include <events/CellIDTAEventConfig.h>

CellIDTAEventConfig::CellIDTAEventConfig(unsigned long t, unsigned long cellID, EventCode code, unsigned long deviceID, NetworkType netType, unsigned int ta): CellIDEventConfig(t,
		cellID, code, deviceID, netType), m_ta{ta} {
	// TODO Auto-generated constructor stub
}

const unsigned int CellIDTAEventConfig::getTA() {
	return m_ta;
}


CellIDTAEventConfig::~CellIDTAEventConfig() {
	// TODO Auto-generated destructor stub
}

