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
#include <iomanip>
#include <sstream>
#include <utility>
#include <Constants.h>

using namespace std;

MobilePhone::MobilePhone(const Map* m, const unsigned long id, Point* initPosition, Agent* holder, const Clock* clock, double powerThreshold, double qualityThreshold, HoldableAgent::CONNECTION_TYPE connType) :
		HoldableAgent(m, id, initPosition, nullptr, clock), m_powerThreshold { Constants::POWER_THRESHOLD }, m_qualityThreshold {
				Constants::QUALITY_THRESHOLD }, m_connType {connType} {
	m_connectedTo = nullptr;
	m_mno = nullptr;
}

MobilePhone::~MobilePhone() {
}

const string MobilePhone::toString() const {
	ostringstream result;

	result << HoldableAgent::toString();
	if(m_mno)
		result << left << setw(15) << m_mno->getId();
	else
		result << left << setw(15) << "No MNO";
	return (result.str());
}

bool MobilePhone::tryConnect() {
	bool connected = false;
//if(getId() == 93)
//	cout << "93 try connect" << endl;
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
		antenna = EMField::instance()->computeMaxPower(p, getMobileOperator()->getId());
	else
		// needs to be at the same MNO
		antenna = EMField::instance()->computeMaxQuality(p, getMobileOperator()->getId());


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
		//antennas need to belong to the same MNO
		vector<pair<Antenna*, double>> antennas = EMField::instance()->getInRangeAntennas(p, threshold, use_power, getMobileOperator()->getId());
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

const MobileOperator* MobilePhone::getMobileOperator() const {
	return m_mno;
}

void MobilePhone::setMobileOperator(MobileOperator* mno) {
	m_mno = mno;
}

