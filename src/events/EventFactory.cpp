/*
 * EventFactory.cpp
 *
 *  Created on: Oct 20, 2020
 *      Author: bogdan
 */

#include <events/CellIDEvent.h>
#include <events/CellIDTAEvent.h>
#include <events/EventCode.h>
#include <events/EventFactory.h>
#include <events/EventType.h>
#include <type_traits>
#include <events/CellIDTAEventConfig.h>
#include <events/CellIDEventConfig.h>
#include <iomanip>
#include <iostream>
using namespace std;

EventFactory::EventFactory() {
	// TODO Auto-generated constructor stub

}

//TODO compute time advance
Event* EventFactory::createEvent(EventConfig* config) {
	Event* result = nullptr;

	if(dynamic_cast<CellIDTAEventConfig*>(config)!=nullptr) {
		cout << "am ajuns si aici" << endl;
		result = new CellIDTAEvent(config);
	} else{
		if(dynamic_cast<CellIDEventConfig*>(config)!=nullptr) {
			cout << "am ajuns si aici 2" << endl;
			result = new CellIDEvent(config);
		}
	}
	return result;
}


EventFactory::~EventFactory() {
	// TODO Auto-generated destructor stub
}
