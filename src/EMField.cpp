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
#include <algorithm>
#include <vector>
#include <AntennaType.h>
#include <iomanip>

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

pair<Antenna*, double> EMField::computeMaxPower(const Point* p) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = numeric_limits<double>::min();
		;
		for (Antenna* a : m_antennas) {
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


pair<Antenna*, double> EMField::computeMaxQuality(const Point* p, unsigned long mnoId) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = numeric_limits<double>::min();
		for (Antenna* a : m_antennas) {
			if(a->getId() != mnoId)
				continue;
			double quality = a->computeSignalQuality(p);
			if (quality > max) {
				max = quality;
				result = make_pair(a, quality);
			}
		}
	}
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

double EMField::connectionLikelihoodGrid(Antenna* a, const Grid* g, unsigned long tileIndex) const {
	Coordinate c = g->getTileCenter(tileIndex);
	c.z = 0; //TODO z = tile elevation
	double s_quality = a->computeSignalQuality(c);
	double result = 0.0;
	result = s_quality / m_sumQuality[tileIndex];

	return (result);
}

void EMField::addAntenna(Antenna* a) {
	m_antennas.push_back(a);
}

vector<pair<Antenna*, double>> EMField::getInRangeAntennas(const Point* p, const double threshold, const bool power, unsigned long mnoId) {
	vector<pair<Antenna*, double>> result;
	unsigned long size = m_antennas.size();
	if (size > 0) {
		for (Antenna*& a : m_antennas) {
			if(a->getId() != mnoId)
				continue;
			double x;
			if (power)
				x = a->computePower(p);
			else
				x = a->computeSignalQuality(p);
			if (x > threshold)
				result.push_back(make_pair(a, x));
		}
	}

	std::sort(result.begin(), result.end(), [](pair<Antenna*, double> &left, pair<Antenna*, double> &right) {
		return (left.second < right.second);
	});

	return (result);
}

bool EMField::isAntennaInRange(const Point* p, Antenna* a, const double threshold, const bool power) {
	bool result = false;
	double ps = 0.0;
	if (power)
		ps = a->computePower(p);
	else
		ps = a->computeSignalQuality(p);
	if (ps > threshold) {
		result = true;
	}
	return (result);
}

vector<double>& EMField::sumSignalQuality(const Grid* grid) {
	for (unsigned long tileIndex = 0; tileIndex < grid->getNoTiles(); tileIndex++) {
		double sum = 0.0;
		if (!(tileIndex % grid->getNoTilesY()))
			cout << endl;
		Coordinate c = grid->getTileCenter(tileIndex);
		c.z = 0; //TODO z should be the elevation
		for (Antenna* a : m_antennas) {
			sum += a->computeSignalQuality(c);
		}
		//cout << "tileIndex " << tileIndex << " tile center " << grid->getTileCenter(tileIndex) << " sum signal quality " << sum  << endl;
		cout << fixed << setw(15) << sum << " ";
		m_sumQuality.push_back(sum);
	}
	cout << endl;
	return m_sumQuality;

}

const double* EMField::getAntennaMin3DbArray() const {
	return m_antennaMin3DbArray;
}

double* EMField::getSd() const {
	return m_sd;
}
