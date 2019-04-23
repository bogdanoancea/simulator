/*
 * Data simulator for mobile phone network events
 *
 * Antenna.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <Antenna.h>
#include <HoldableAgent.h>
#include <geos/geom/GeometryFactory.h>
#include <Map.h>
#include <iomanip>
#include <sstream>

using namespace std;

Antenna::Antenna(Map* m, long id, Point* initPosition, double attenuationFactor, double power, int maxConnections) :
		ImmovableAgent(m, id, initPosition), m_power { power }, m_attenuationFactor {
			attenuationFactor }, m_maxConnections { maxConnections }, m_numActiveConnections {
				0 } {
	m_cell = this->getMap()->getGlobalFactory()->createPolygon();
}

Antenna::~Antenna() {
	// TODO Auto-generated destructor stub
}

double Antenna::getAttenuationFactor() const {
	return m_attenuationFactor;
}

void Antenna::setAttenuationFactor(double attenuationFactor) {
	m_attenuationFactor = attenuationFactor;
}

Polygon* Antenna::getCell() const {
	return m_cell;
}

void Antenna::setCell(const Polygon& cell) {
	//m_cell = cell;
}

string Antenna::toString() {
	ostringstream result;
	result << ImmovableAgent::toString() << left << setw(15) << m_power << setw(25) << m_maxConnections << setw(15)
			<< m_attenuationFactor;
	return (result.str());
}


double Antenna::getPower() const {
	return m_power;
}

void Antenna::setPower(double power) {
	m_power = power;
}

int Antenna::getMaxConnections() const {
	return (m_maxConnections);
}

void Antenna::setMaxConnections(int maxConnections) {
	m_maxConnections = maxConnections;
}

bool Antenna::tryConnect(HoldableAgent* ag) {
	bool result = false;
	if (m_numActiveConnections < m_maxConnections) {
		attachDevice(ag);
		m_numActiveConnections++;
	}
	return (result);
}

void Antenna::attachDevice(HoldableAgent* device) {
	m_devices.push_back(device);
}


void Antenna::dettachDevice(HoldableAgent* device) {
	vector<HoldableAgent*>::iterator it = std::find(m_devices.begin(), m_devices.end(), device);
	if (it != m_devices.end()) // == myVector.end() means the element was not found
		m_devices.erase(it);
}
