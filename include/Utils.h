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

#include <geos/geom/Point.h>
#include <cmath>
#include <vector>
#include <tinyxml2.h>

class Map;

using namespace geos;
using namespace geos::geom;
using namespace std;
using namespace tinyxml2;

namespace utils {

	vector<Point*> generatePoints(Map* m, int n);
	void printPersonHeader();
	void printAntennaHeader();
	void printPhoneHeader();
	const double PI = std::atan(1.0) * 4;
	const char* getAntennasFile(int argc, char** argv);
	XMLNode* getNode(XMLElement* el, const char* name);
	XMLElement* getFirstChildElement(XMLElement* el, const char* name) noexcept(false);
}

#endif
