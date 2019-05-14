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

MobilePhone::MobilePhone(Map* m, long id, Point* initPosition, Agent* holder, Clock* clock, double powerThreshold, double qualityThreshold) :
		HoldableAgent(m, id, initPosition, nullptr, clock), m_powerThreshold { Constants::POWER_THRESHOLD }, m_qualityThreshold {
				Constants::QUALITY_THRESHOLD } {
	m_connectedTo = nullptr;
}

MobilePhone::~MobilePhone() {
	// TODO Auto-generated destructor stub
}

string MobilePhone::toString() {
	return (HoldableAgent::toString());
}

bool MobilePhone::tryConnect(HoldableAgent::CONNECTION_TYPE type) {
	return tryConnectNaiveAlgorithm(type);
}

bool MobilePhone::tryConnectNaiveAlgorithm(HoldableAgent::CONNECTION_TYPE type) {
	bool connected = false;

	//select the most powerful antenna
	Point *p = this->getLocation();
	bool use_power = (type == HoldableAgent::CONNECTION_TYPE::USING_POWER);
	double threshold = (use_power ? m_powerThreshold : Constants::QUALITY_THRESHOLD);

	if (m_connectedTo != nullptr) {
		//check if this antenna is still in range, otherwise detach from it
		bool inRange = EMField::instance()->isAntennaInRange(p, m_connectedTo, threshold, use_power);
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
		if (m_connectedTo != nullptr && antenna.first->getId() != m_connectedTo->getId()) {
			m_connectedTo->dettachDevice(this);
		}
		m_connectedTo = antenna.first;
	}
	else {
		//try to connect to another antenna in range
		vector<pair<Antenna*, double>> antennas = EMField::instance()->getInRangeAntennas(p, threshold, use_power);
		unsigned long size = antennas.size();
		for (unsigned long i = 0; i < size; i++) {
			connected = antennas[i].first->tryRegisterDevice(this);
			if (connected) {
				if (m_connectedTo != nullptr && antennas[i].first->getId() != m_connectedTo->getId()) {
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

double MobilePhone::getQualityThreshold() const {
	return m_qualityThreshold;
}

void MobilePhone::setQualityThreshold(double qualityThreshold) {
	m_qualityThreshold = qualityThreshold;
}

double MobilePhone::getPowerThreshold() const {
	return m_powerThreshold;
}

void MobilePhone::setPowerThreshold(double powerThreshold) {
	m_powerThreshold = powerThreshold;
}
