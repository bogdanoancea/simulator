/*
 * Data simulator for mobile phone network events
 *
 * EMField.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Bogdan Oancea
 */

#include <EMField.h>
#include <geos/geom/Point.h>
#include <cmath>

#include <vector>

EMField* EMField::m_instance = nullptr;

EMField::EMField() {
	// TODO Auto-generated constructor stub
}

pair<Antenna*, double> EMField::computeMaxPower(Point* p) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	int size = m_antennas.size();
	if (size > 0) {
		double max = -1;
		for (Antenna* a : m_antennas) {
			double power = a->getPower() * pow(p->distance(a->getLocation()), -a->getAttenuationFactor());
			if (power > max) {
				max = power;
				result = make_pair(a, power);
			}

		}
	}
	return (result);
}

void EMField::addAntenna(Antenna* a) {
	m_antennas.push_back(a);
}

vector<Antenna*> EMField::getActiveAntennas(Point p) {
	vector<Antenna*> result;

	return (result);
}
