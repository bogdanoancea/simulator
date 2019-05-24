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

MobilePhone::MobilePhone(const Map* m, long id, Point* initPosition, Agent* holder, Clock* clock, double powerThreshold, double qualityThreshold, HoldableAgent::CONNECTION_TYPE connType) :
		HoldableAgent(m, id, initPosition, nullptr, clock), m_powerThreshold { Constants::POWER_THRESHOLD }, m_qualityThreshold {
				Constants::QUALITY_THRESHOLD }, m_connType {connType} {
	m_connectedTo = nullptr;
}

MobilePhone::~MobilePhone() {
}

string MobilePhone::toString() const {
	return (HoldableAgent::toString());
}

bool MobilePhone::tryConnect() {
	bool connected = false;

	//select the most powerful antenna
	Point *p = this->getLocation();
	bool use_power = (m_connType == HoldableAgent::CONNECTION_TYPE::USING_POWER);
	double threshold = (use_power ? m_powerThreshold : m_qualityThreshold);

	if (m_connectedTo != nullptr) {
		//check if this antenna is still in range, otherwise detach from it
		bool inRange = EMField::instance()->isAntennaInRange(p, m_connectedTo, threshold, use_power);
		if (!inRange) {
			m_connectedTo->dettachDevice(this);
			m_connectedTo = nullptr;
			connected = false;
		}
	}
	pair<Antenna*, double> antenna;
	if (use_power)
		antenna = EMField::instance()->computeMaxPower(p);
	else
		antenna = EMField::instance()->computeMaxQuality(p);


	if (antenna.second > threshold) {
		connected = antenna.first->tryRegisterDevice(this);
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
	return (m_qualityThreshold);
}

void MobilePhone::setQualityThreshold(double qualityThreshold) {
	m_qualityThreshold = qualityThreshold;
}

double MobilePhone::getPowerThreshold() const {
	return (m_powerThreshold);
}

void MobilePhone::setPowerThreshold(double powerThreshold) {
	m_powerThreshold = powerThreshold;
}

const string MobilePhone::getName() const {
	return ("MobilePhone");
}

