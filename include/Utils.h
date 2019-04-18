/*
 * Data simulator for mobile phone network events
 *
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#ifndef UTILS_H
#define UTILS_H

#include <geos/geom/Coordinate.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <Map.h>
#include <iomanip>
#include <random>

#include <iostream>
#include <vector>

using namespace geos;
using namespace geos::geom;
using namespace std;

namespace utils {

	vector<Point*> generatePoints(Map* m, std::mt19937 generator, int n);
	void printPersonHeader();
	void printAntennaHeader();
	void printPhoneHeader();
	double drand(double min, double max, std::mt19937 generator);
}

#endif
