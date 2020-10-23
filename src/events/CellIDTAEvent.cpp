/*
 * CellIDTAEvent.cpp
 *
 *  Created on: Oct 20, 2020
 *      Author: bogdan
 */

#include <events/CellIDTAEvent.h>
#include <Constants.h>
#include <sstream>
#include <string>
#include <Utils.h>

using namespace utils;

CellIDTAEvent::CellIDTAEvent(EventConfig* config) : m_config{config}{
	// TODO Auto-generated constructor stub
}

EventConfig* CellIDTAEvent::getEventConfig() {
	return m_config;
}


const string CellIDTAEvent::toString() {
	std::stringstream result;
	CellIDTAEventConfig* tmp = static_cast<CellIDTAEventConfig*>( m_config);
	//cout << "trec pe aici" << endl;
	result <<tmp->getTimestamp() << Constants::sep << tmp->getCellID() << Constants::sep << static_cast<int>(tmp->getEventCode())
			<< Constants::sep << tmp->getDeviceID() << Constants::sep << utils::toString(tmp->getNetworkType()) << Constants::sep << tmp->getTA();

	return result.str();
}


CellIDTAEvent::~CellIDTAEvent() {
	delete m_config;
}

