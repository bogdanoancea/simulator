/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * MobilePhone.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/Antenna.h>
#include <agent/MobileOperator.h>
#include <agent/MobilePhone.h>
#include <EMField.h>
#include <iomanip>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

MobilePhone::MobilePhone(const Map* m, const unsigned long id, Point* initPosition, Agent* holder, const Clock* clock, double threshold, HoldableAgent::CONNECTION_TYPE connType) :
		HoldableAgent(m, id, initPosition, nullptr, clock), m_threshold { threshold }, m_connType { connType } {
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

	if (antenna.first != nullptr && antenna.second > m_threshold) {
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
		vector<pair<Antenna*, double>> antennas = EMField::instance()->getInRangeAntennas(p, m_threshold, m_connType, getMobileOperator()->getId());
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


const MobileOperator* MobilePhone::getMobileOperator() const {
	return (m_mno);
}

void MobilePhone::setMobileOperator(MobileOperator* mno) {
	m_mno = mno;
}

const string MobilePhone::getHeader()  {
	ostringstream result;
	result << left << setw(15) << "Phone ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Speed " << setw(15) << " Owner id " << setw(15) << "MNO Id" << endl;
	return result.str();
}


