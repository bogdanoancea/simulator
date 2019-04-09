/*
 * Data simulator for mobile phone network events
 *
 * Antenna.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <Antenna.h>

Antenna::Antenna() {
	// TODO Auto-generated constructor stub

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

const Polygon& Antenna::getCell() const {
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
