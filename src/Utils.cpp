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
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/Antenna.h>
#include <agent/MobilePhone.h>
#include <EMField.h>
#include <EventType.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/GeometryFactory.h>
#include <map/Map.h>
#include <RandomNumberGenerator.h>
#include <TinyXML2.h>
#include <Utils.h>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace utils {
using namespace geos;
using namespace geos::geom;
using namespace std;

//TODO it is a mess, this function should be written again.
vector<Point*> generatePoints(const Map* m, unsigned long n, double percentHome, unsigned seed) {
	vector<Point*> result;
	RandomNumberGenerator* random_generator;
	random_generator = RandomNumberGenerator::instance(seed);

	Geometry* g = m->getBoundary();
	if (g != nullptr) {
		const Envelope* env = g->getEnvelopeInternal();
		double xmin = env->getMinX();
		double xmax = env->getMaxX();
		double ymin = env->getMinY();
		double ymax = env->getMaxY();

		unsigned long nhome = (unsigned long) n * percentHome;
		double* x1 = random_generator->generateUniformDouble(xmin, xmax, n);
		double* y1 = random_generator->generateUniformDouble(ymin, ymax, n);

		unsigned long i = 0, k = 0;
		while (k < nhome) {
			if (x1[i] < 1e-15)
				x1[i] = 0.0;
			if (y1[i] < 1e-15)
				y1[i] = 0.0;
			Coordinate c = Coordinate(x1[i], y1[i]);
			c.z = 0;
			i++;
			Point* p = m->getGlobalFactory()->createPoint(c);
			if (g->contains(p)) {
				result.push_back(p);
				k++;
			} else
				m->getGlobalFactory()->destroyGeometry(p);
			// we used all the numbers, generate others
			if (i == n - 1) {
				delete[] x1;
				delete[] y1;
				x1 = random_generator->generateUniformDouble(xmin, xmax, n);
				y1 = random_generator->generateUniformDouble(ymin, ymax, n);
				if (x1[i] < 1e-15)
					x1[i] = 0.0;
				if (y1[i] < 1e-15)
					y1[i] = 0.0;
				i = 0;
			}
		}
		delete[] x1;
		delete[] y1;
		random_generator->setSeed(time(0));
		double* x2 = random_generator->generateUniformDouble(xmin, xmax, n);
		double* y2 = random_generator->generateUniformDouble(ymin, ymax, n);

		k = 0;
		i = 0;
		while (k < n - nhome) {
			if (x2[i] < 1e-15)
				x2[i] = 0.0;
			if (y2[i] < 1e-15)
				y2[i] = 0.0;
			Coordinate c = Coordinate(x2[i], y2[i]);
			c.z = 0;
			i++;
			Point* p = m->getGlobalFactory()->createPoint(c);
			if (g->contains(p)) {
				result.push_back(p);
				k++;
			} else
				m->getGlobalFactory()->destroyGeometry(p);

			// we used all the numbers, generate others
			if (i == n - 1) {
				delete[] x2;
				delete[] y2;
				x2 = random_generator->generateUniformDouble(xmin, xmax, n);
				y2 = random_generator->generateUniformDouble(ymin, ymax, n);
				if (x2[i] < 1e-15)
					x2[i] = 0.0;
				if (y2[i] < 1e-15)
					y2[i] = 0.0;
				i = 0;
			}
		}
		delete[] x2;
		delete[] y2;
	}
	return (result);
}

vector<Point*> generateFixedPoints(const Map* m, unsigned long n, unsigned seed) {
	vector<Point*> result;
	RandomNumberGenerator* random_generator;
	random_generator = RandomNumberGenerator::instance(seed);

	Geometry* g = m->getBoundary();
	if (g != nullptr) {
		const Envelope* env = g->getEnvelopeInternal();
		double xmin = env->getMinX();
		double xmax = env->getMaxX();
		double ymin = env->getMinY();
		double ymax = env->getMaxY();

		//generate a pool of numbers to choose from
		double* x1 = random_generator->generateUniformDouble(xmin, xmax, n);
		double* y1 = random_generator->generateUniformDouble(ymin, ymax, n);

		unsigned long i = 0, k = 0;
		while (n) {
			Coordinate c = Coordinate(x1[i], y1[i]);
			c.z = 0;
			i++;
			Point* p = m->getGlobalFactory()->createPoint(c);
			if (g->contains(p)) {
				result.push_back(p);
				k++;
			} else
				m->getGlobalFactory()->destroyGeometry(p);

			// we used all the numbers, generate others
			if (i == n - 1) {
				delete[] x1;
				delete[] y1;
				x1 = random_generator->generateUniformDouble(xmin, xmax, n - k + 50);
				y1 = random_generator->generateUniformDouble(ymin, ymax, n - k + 50);
				i = 0;
			}
		}
		delete[] x1;
		delete[] y1;

	}
	return (result);
}

XMLNode* getNode(XMLElement* el, const char* name) {
	XMLNode* n = nullptr;
	XMLElement* element = el->FirstChildElement(name);
	if (element) {
		n = element->FirstChild();
		if (!n)
			throw std::runtime_error("Syntax error in the configuration file ");
	}
	return (n);
}

XMLElement* getFirstChildElement(XMLElement* el, const char* name) {
	XMLElement* n = el->FirstChildElement(name);
	if (!n)
		throw std::runtime_error("Syntax error in the configuration file ");
	return (n);
}

double getValue(XMLElement* el, const char* name, double default_value) {
	double result = default_value;
	XMLNode* n = getNode(el, name);
	if (n)
		result = atof(n->ToText()->Value());
	return result;
}

unsigned long getValue(XMLElement* el, const char* name, unsigned long default_value) {
	long result = default_value;
	XMLNode* n = getNode(el, name);
	if (n)
		result = atol(n->ToText()->Value());
	return result;
}

const char* getValue(XMLElement* el, const char* name, const char* default_value) {
	const char* result = default_value;
	XMLNode* n = getNode(el, name);
	if (n)
		result = n->ToText()->Value();
	;
	return result;
}

int getValue(XMLElement* el, const char* name, int default_value) {
	int result = default_value;
	XMLNode* n = getNode(el, name);
	if (n)
		result = atoi(n->ToText()->Value());
	return result;
}

double getValue(XMLElement* el, const char* name) {
	double result;
	XMLNode* n = getNode(el, name);
	if (n)
		result = atof(n->ToText()->Value());
	else {
		string msg = string { "no value for " } + string { name };
		throw runtime_error(msg);
	}
	return result;
}

//vector<double> computeProbability(Map* map, unsigned long t, MobilePhone* m, vector<AntennaInfo>& data, pair<um_iterator, um_iterator> antennas_iterator, PriorType prior) {
//	vector<double> result;
//	// take the mobile phone and see which is the antenna connected to
//	vector<AntennaInfo>::iterator ai;
//	bool found = false;
//	for (vector<AntennaInfo>::iterator i = data.begin(); i != data.end(); i++) {
//		ai = i;
//		if (ai->getTime() == t && ai->getDeviceId() == m->getId()
//				&& (ai->getEventCode() == static_cast<int>(EventType::ATTACH_DEVICE) || ai->getEventCode() == static_cast<int>(EventType::ALREADY_ATTACHED_DEVICE))) {
//			found = true;
//			break;
//		}
//	}
//	if (prior == PriorType::NETWORK)
//		result = useNetworkPrior(map, found, ai, antennas_iterator);
//	else if (prior == PriorType::UNIFORM)
//		result = useUniformPrior(map, found, ai, antennas_iterator);
//	return (result);
//}

//vector<double> useNetworkPrior(Map* map, bool connected, vector<AntennaInfo>::iterator ai, pair<um_iterator, um_iterator> antennas_iterator)  {
//	vector<double> result;
//	double sum = 0.0;
//	for (unsigned long tileIndex = 0; tileIndex < map->getNoTiles(); tileIndex++) {
//		double lh = 0.0;
//		if (connected) {
//			Coordinate c = map->getTileCenter(tileIndex);
//			unsigned long antennaId = ai->getAntennaId();
//			Antenna* a = nullptr;
//			for (auto it = antennas_iterator.first; it != antennas_iterator.second; it++) {
//				a = dynamic_cast<Antenna*>(it->second);
//				if (a->getId() == antennaId) {
//					break;
//				}
//			}
//			c.z = 0; //TODO tile elevation!
//			if (a != nullptr) {
//				lh = a->computeSignalQuality(c);
//				sum += lh;
//			}
//		}
//		result.push_back(lh);
//	}
//	for (auto& i : result) {
//		if (sum != 0.0) {
//			i /= sum;
//		}
//	}
//	return result;
//}

//vector<double> useUniformPrior(Map* map, bool connected, vector<AntennaInfo>::iterator ai, pair<um_iterator, um_iterator> antennas_iterator)  {
//	vector<double> result;
//
//	for (unsigned long tileIndex = 0; tileIndex < map->getNoTiles(); tileIndex++) {
//		double lh = 0.0;
//		if (connected) {
//			unsigned long antennaId = ai->getAntennaId();
//			Antenna* a = nullptr;
//			for (auto it = antennas_iterator.first; it != antennas_iterator.second; it++) {
//				a = dynamic_cast<Antenna*>(it->second);
//				if (a->getId() == antennaId) {
//					break;
//				}
//			}
//			if (a != nullptr) {
//				lh = EMField::instance()->connectionLikelihoodGrid(a, map, tileIndex);
//			}
//		}
//		result.push_back(lh);
//	}
//	for (auto& i : result) {
//		if (i > 0.0)
//			i /= (map->getNoTilesX() * map->getNoTilesY());
//		else
//			i = 1.0 / (map->getNoTilesX() * map->getNoTilesY());
//	}
//	return result;
//}
}
