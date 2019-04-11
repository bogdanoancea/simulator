/*
 * Data simulator for mobile phone network events
 *
 * Antenna.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include "Antenna.h"
#include "ImmovableAgent.h"

Antenna::Antenna(Map* m, long id, Point* initPosition) :
		ImmovableAgent(m, id, initPosition), m_power { 0.0 }, m_attenuationFactor {
				0.0 }, m_maxConnections { 0 } {
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

double Antenna::getPower() const {
	return m_power;
}

void Antenna::setPower(double power) {
	m_power = power;
}

int Antenna::getMaxConnections() const {
	return m_maxConnections;
}

void Antenna::setMaxConnections(int maxConnections) {
	m_maxConnections = maxConnections;
}
