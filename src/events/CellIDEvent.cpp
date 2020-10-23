/*
 * CellIDEvent.cpp
 *
 *  Created on: Oct 20, 2020
 *      Author: bogdan
 */

#include <Constants.h>
#include <events/CellIDEvent.h>
#include <events/CellIDEventConfig.h>
#include <sstream>
#include <string>
#include <Utils.h>

using namespace utils;

CellIDEvent::CellIDEvent(EventConfig* config): m_config{config} {
}


const string CellIDEvent::toString() {
	std::stringstream ss;
	CellIDEventConfig* tmp = static_cast<CellIDEventConfig*>( m_config);
	ss <<tmp->getTimestamp() << Constants::sep << tmp->getCellID() << Constants::sep << static_cast<int>(tmp->getEventCode()) << Constants::sep <<
			tmp->getDeviceID() << Constants::sep << utils::toString(tmp->getNetworkType());
	return ss.str();
}


EventConfig* CellIDEvent::getEventConfig() {
	return m_config;
}


CellIDEvent::~CellIDEvent() {
	// TODO Auto-generated destructor stub
	delete m_config;
}

