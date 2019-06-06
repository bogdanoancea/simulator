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
}

pair<Antenna*, double> EMField::computeMaxPower(const Point* p) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = numeric_limits<double>::min();;
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

pair<Antenna*, double> EMField::computeMaxQuality(const Point* p) {
	pair<Antenna*, double> result { nullptr, 0.0 };
	unsigned long size = m_antennas.size();
	if (size > 0) {
		double max = numeric_limits<double>::min();
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

double EMField::connectionLikelihood(Antenna* a, const Point * p) {
	double s_quality = a->computeSignalQuality(p);
	double result = 0.0, sum = 0.0;
	//unsigned long size = m_antennas.size();
	//if (size > 0) {
		for (Antenna*& a : m_antennas) {
			sum += a->computeSignalQuality(p);
		}
	//}
	cout << " field quality " << sum << endl;
	if (sum)
		result = s_quality / sum;

	return (result);
}

void EMField::addAntenna(Antenna* a) {
	m_antennas.push_back(a);
}

vector<pair<Antenna*, double>> EMField::getInRangeAntennas(const Point* p, const double threshold, const bool power) {
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

vector<double> EMField::sumSignalQuality(Grid* grid){
	vector<double> result;


	return result;

}
