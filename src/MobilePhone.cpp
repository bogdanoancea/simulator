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

MobilePhone::MobilePhone(const Map* m, const unsigned long id, Point* initPosition, Agent* holder, const Clock* clock,
		double threshold, HoldableAgent::CONNECTION_TYPE connType) :
		HoldableAgent(m, id, initPosition, nullptr, clock), m_threshold{threshold}, m_connType { connType } {
	m_connectedTo = nullptr;
	m_mno = nullptr;
}

MobilePhone::~MobilePhone() {
}

const string MobilePhone::toString() const {
	ostringstream result;

	result << HoldableAgent::toString();
	if (m_mno)
		result << left << setw(15) << m_mno->getId();
	else
		result << left << setw(15) << "No MNO";
	return (result.str());
}

bool MobilePhone::tryConnect() {
	bool connected = false;
	//select the most powerful antenna
	Point *p = this->getLocation();
	if (m_connectedTo != nullptr) {
		//check if this antenna is still in range, otherwise detach from it
		bool inRange = EMField::instance()->isAntennaInRange(p, m_connectedTo, m_threshold, m_connType);
		if (!inRange) {
			m_connectedTo->dettachDevice(this);
			m_connectedTo = nullptr;
			connected = false;
		}
	}

	pair<Antenna*, double> antenna;
	if (m_connType == HoldableAgent::CONNECTION_TYPE::USING_POWER)
		antenna = EMField::instance()->computeMaxPower(p, getMobileOperator()->getId());
	else if (m_connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY) {
		antenna = EMField::instance()->computeMaxQuality(p, getMobileOperator()->getId());
	} else if (m_connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH) {
		antenna = EMField::instance()->computeMaxStrength(p, getMobileOperator()->getId());
	}

	if (antenna.second > m_threshold) {
		connected = antenna.first->tryRegisterDevice(this);
	}
	if (connected) {
		if (m_connectedTo != nullptr && antenna.first->getId() != m_connectedTo->getId()) {
			m_connectedTo->dettachDevice(this);
		}
		m_connectedTo = antenna.first;
	} else {
		//try to connect to another antenna in range
		//antennas need to belong to the same MNO
		vector<pair<Antenna*, double>> antennas = EMField::instance()->getInRangeAntennas(p, m_threshold, m_connType,
				getMobileOperator()->getId());
		unsigned long size = antennas.size();
		for (unsigned long i = 0; i < size; i++) {
			connected = antennas[i].first->tryRegisterDevice(this);
			if (connected) {
				if (m_connectedTo != nullptr && antennas[i].first->getId() != m_connectedTo->getId()) {
					m_connectedTo->dettachDevice(this);
				}
				m_connectedTo = antennas[i].first;
				break;
			}
		}
	}
	return (connected);
}

const string MobilePhone::getName() const {
	return ("MobilePhone");
}

const MobileOperator* MobilePhone::getMobileOperator() const {
	return (m_mno);
}

void MobilePhone::setMobileOperator(MobileOperator* mno) {
	m_mno = mno;
}

