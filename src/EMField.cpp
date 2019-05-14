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

EMField* EMField::m_instance = nullptr;

EMField::EMField() {
	// TODO Auto-generated constructor stub
}

pair<Antenna*, double> EMField::computeMaxPower(Point* p) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = -1;
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

pair<Antenna*, double> EMField::computeMaxQuality(Point* p) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = -1;
		for (Antenna* a : m_antennas) {
			if (a->getType() == AntennaType::OMNIDIRECTIONAL) {
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

double EMField::connectionLikelihood(Antenna* a, Point * p) {
	double s_quality = a->computeSignalQuality(p);
	double result = 0.0, sum = 0.0;
	unsigned long size = m_antennas.size();
	if (size > 0) {
		for (Antenna*& a : m_antennas) {
			sum += a->computeSignalQuality(p);
		}
	}
	if (sum)
		result = s_quality / sum;

	return (result);
}

void EMField::addAntenna(Antenna* a) {
	m_antennas.push_back(a);
}

vector<pair<Antenna*, double>> EMField::getInRangeAntennas(Point* p, double threshold, bool power) {
	vector<pair<Antenna*, double>> result;
	unsigned long size = m_antennas.size();
	if (size > 0) {
		for (Antenna*& a : m_antennas) {
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

bool EMField::isAntennaInRange(Point* p, Antenna* a, double threshold, bool power) {
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

