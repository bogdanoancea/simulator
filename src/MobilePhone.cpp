/*
 * Data simulator for mobile phone network events
 *
 * MobilePhone.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <MobilePhone.h>
#include <EMField.h>
#include <iostream>
#include <utility>
#include <Constants.h>

using namespace std;

MobilePhone::MobilePhone(Map* m, long id, Point* initPosition, Agent* holder, Clock* clock, double powerThreshold) :
		HoldableAgent(m, id, initPosition, nullptr, clock), m_powerThreshold { Constants::POWER_THRESHOLD } {
	m_connectedTo = nullptr;
}

MobilePhone::~MobilePhone() {
	// TODO Auto-generated destructor stub
}

string MobilePhone::toString() {
	return (HoldableAgent::toString());
}

bool MobilePhone::tryConnect() {
	return tryConnectNaiveAlgorithm();
}

bool MobilePhone::tryConnectNaiveAlgorithm() {
	bool connected = false;

	//select the most powerful antenna
	Point *p = this->getLocation();
	if (m_connectedTo != nullptr) {
		//check if this antenna is still in range, otherwise detach from it
		bool inRange = EMField::instance()->isAntennaInRange(p, m_connectedTo, m_powerThreshold);
		if (!inRange) {
			m_connectedTo->dettachDevice(this);
			m_connectedTo = nullptr;
			connected = false;
		}
	}

	pair<Antenna*, double> antenna = EMField::instance()->computeMaxPower(p);

	if (antenna.second > m_powerThreshold) {
		connected = antenna.first->tryRegisterDevice(this);
		//cout << "m-am conectat la antena:" << antenna.first->getId() << endl;
		//cout << "tre sa ma detasez de la antena:" << m_connectedTo->getId() << endl;
	}
	if (connected) {
		if (m_connectedTo!= nullptr && antenna.first->getId() != m_connectedTo->getId()) {
			m_connectedTo->dettachDevice(this);
		}
		m_connectedTo = antenna.first;
	}
	else {
		//try to connect to another antenna in range
		vector<pair<Antenna*, double>> antennas = EMField::instance()->getInRangeAntennas(p);
		unsigned long size = antennas.size();
		for (unsigned long i = 0; i < size; i++) {
			connected = antennas[i].first->tryRegisterDevice(this);
			if (connected) {
				if (m_connectedTo!= nullptr && antennas[i].first->getId() != m_connectedTo->getId()) {
					m_connectedTo->dettachDevice(this);
				}
				//m_connectedTo->dettachDevice(this);
				m_connectedTo = antennas[i].first;
				break;
			}
		}
	}
	return (connected);
}
