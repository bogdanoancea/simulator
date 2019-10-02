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
 * EMField.cpp
 *
 *  Created on: Apr 23, 2019
 *      Author: Bogdan Oancea
 */

#include <EMField.h>
#include <geos/geom/Point.h>
#include <cmath>
#include <algorithm>
#include <vector>
#include <AntennaType.h>
#include <iomanip>
#include <limits>

EMField* EMField::m_instance = nullptr;

EMField::EMField() {
	m_antennaMin3DbArray = new double[Constants::ANTENNA_MIN_3_DB];
	for (unsigned int i = 0; i < Constants::ANTENNA_MIN_3_DB; i++) {
		m_antennaMin3DbArray[i] = i * 180.0 / (Constants::ANTENNA_MIN_3_DB - 1.0);
	}
	m_sd = new double[Constants::ANTENNA_MAPPING_N];
	for (unsigned int i = 0; i < Constants::ANTENNA_MAPPING_N; i++) {
		m_sd[i] = 180.0 / Constants::ANTENNA_MAPPING_N + i * 180.0 / Constants::ANTENNA_MAPPING_N;
	}
}

EMField::~EMField() {
	delete[] m_antennaMin3DbArray;
	delete[] m_sd;

}

//TODO Directional antennas
pair<Antenna*, double> EMField::computeMaxPower(const Point* p, const unsigned long mnoId) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = -1;
		for (Antenna* a : m_antennas) {
			if (a->getMNO()->getId() != mnoId)
				continue;
			if (a->getType() == AntennaType::OMNIDIRECTIONAL) {
				double power = a->computePower(p);
				if (power > max) {
					max = power;
					result = make_pair(a, power);
				}
			}
		}
	}
	return (result);
}


pair<Antenna*, double> EMField::computeMaxQuality(const Point* p, const unsigned long mnoId) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = -1.0;
		for (Antenna* a : m_antennas) {
			if (a->getMNO()->getId() != mnoId)
				continue;
			if (a->getLocation()->distance(p) <= a->getRmax()) {
				double quality = a->computeSignalQuality(p);
				if (quality > max) {
					max = quality;
					result = make_pair(a, quality);
				}
			}
		}
	}
	return (result);
}


pair<Antenna*, double> EMField::computeMaxStrength(const Point* p, const unsigned long mnoId) {
	pair<Antenna*, double> result  = { nullptr, -numeric_limits<double>::max() };
//	cout << "am intrat in compute max strength cu mnoid = " << mnoId << " si p = " << p->toString() <<endl;
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = -numeric_limits<double>::max();
		for (Antenna* a : m_antennas) {
			if (a->getMNO()->getId() != mnoId)
				continue;
//			cout << "antenna " << a->getId() << endl;
			if (a->getLocation()->distance(p) <= a->getRmax()) {
//				cout << "aici " << a->getId() << endl;
				double strength = a->computeSignalStrength(p);
//				cout << strength << endl;
				if (strength > max) {
					max = strength;
					result = make_pair(a, strength);
				}
			}
		}
	}
//	cout << "returnez "<< result.second << endl;
	return (result);
}


double EMField::connectionLikelihood(Antenna* a, const Point * p) {
	double s_quality = a->computeSignalQuality(p);
	double result = 0.0, sum = 0.0;
	for (Antenna*& a : m_antennas) {
		sum += a->computeSignalQuality(p);
	}
	if (sum)
		result = s_quality / sum;

	return (result);
}

double EMField::connectionLikelihoodGrid(Antenna* a, const Grid* g, unsigned long tileIndex) {
	Coordinate c = g->getTileCenter(tileIndex);
	c.z = 0; //TODO z = tile elevation
	double s_quality = a->computeSignalQuality(c);
	unsigned long mnoID = a->getMNO()->getId();
	double result = 0.0;
	vector<double> sumQuality = m_sumQuality[mnoID];
	result = s_quality / sumQuality[tileIndex];
	return (result);
}

void EMField::addAntenna(Antenna* a) {
	m_antennas.push_back(a);
}

vector<pair<Antenna*, double>> EMField::getInRangeAntennas(const Point* p, const double threshold, const HoldableAgent::CONNECTION_TYPE connType, unsigned long mnoId) {
	vector<pair<Antenna*, double>> result;
	unsigned long size = m_antennas.size();
	if (size > 0) {
		for (Antenna*& a : m_antennas) {
			if (a->getMNO()->getId() != mnoId)
				continue;
			if (a->getRmax() >= a->getLocation()->distance(p)) {
				double x = 0.0;
				if (connType == HoldableAgent::USING_POWER)
					x = a->computePower(p);
				else if(connType == HoldableAgent::USING_SIGNAL_QUALITY)
					x = a->computeSignalQuality(p);
				else if(connType == HoldableAgent::USING_SIGNAL_STRENGTH)
					x = a->computeSignalStrength(p);

				if (x > threshold)
					result.push_back(make_pair(a, x));
			}
		}
	}
	if (result.size() > 0)
		sort(result.begin(), result.end(), [](const pair<Antenna*, double> &left, const pair<Antenna*, double> &right) {
			return (left.second < right.second);
		});

	return (result);
}

bool EMField::isAntennaInRange(const Point* p, Antenna* a, const double threshold, const HoldableAgent::CONNECTION_TYPE connType) {
	bool result = false;
	double ps = 0.0;
	if (connType == HoldableAgent::USING_POWER)
		ps = a->computePower(p);
	else if(connType == HoldableAgent::USING_SIGNAL_QUALITY) {
		if (a->getRmax() >= a->getLocation()->distance(p))
			ps = a->computeSignalQuality(p);
	}
	else if(connType == HoldableAgent::USING_SIGNAL_STRENGTH) {
		if(a->getRmax() >= a->getLocation()->distance(p))
			ps = a->computeSignalStrength(p);
	}

	if (ps > threshold) {
		result = true;
	}
	return (result);
}

vector<double> EMField::sumSignalQuality(const Grid* grid, const unsigned long mnoID) {
	vector<double> tmp;
	for (unsigned long tileIndex = 0; tileIndex < grid->getNoTiles(); tileIndex++) {
		double sum = 0.0;
		if (!(tileIndex % grid->getNoTilesY()))
			cout << endl;
		Coordinate c = grid->getTileCenter(tileIndex);
		c.z = 0; //TODO z should be the elevation
		for (Antenna* a : m_antennas) {
			if (a->getMNO()->getId() != mnoID)
				continue;
			sum += a->computeSignalQuality(c);
		}
		tmp.push_back(sum);
		//cout << "tileIndex " << tileIndex << " tile center " << grid->getTileCenter(tileIndex) << " sum signal quality " << sum  << endl;
		cout << fixed << setw(15) << sum << " ";
	}
	m_sumQuality.insert(pair<const unsigned long, vector<double>>(mnoID, tmp));
	cout << endl;
	return tmp;
}

const double* EMField::getAntennaMin3DbArray() const {
	return m_antennaMin3DbArray;
}

double* EMField::getSd() const {
	return m_sd;
}
