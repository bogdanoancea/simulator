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
#include <events/EventCode.h>
#include <parsers/HomeWorkLocation.h>
#include <EMField.h>
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
#include <limits>
#include <sstream>
#include <filesystem>
#include <vector>

namespace utils {
using namespace geos;
using namespace geos::geom;
using namespace std;

//TODO it is a mess, this function should be written again.
vector<Point*> generatePoints(SimulationConfiguration* sc, unsigned long n, double percentHome) {
	if(sc->isHomeWorkScenario()) {
		return generateHomeWorkPoints(sc, n, percentHome);
	}
	else {
		return generateRandomPoints(sc, n, percentHome);
	}
}


vector<Point*> generateRandomPoints(SimulationConfiguration* sc, unsigned long n, double percentHome) {
	vector<Point*> result;
	Map* m = sc->getMap();
	RandomNumberGenerator* random_generator;
	unsigned int seed = sc->getSeed();
	random_generator = RandomNumberGenerator::instance(seed);

	Geometry* g = m->getBoundary();
	if (g != nullptr) {
		unsigned long nhome = (unsigned long) n * percentHome;
		vector<Point*> r = generateFixedPoints(m , nhome, random_generator);
		std::move (r.begin(), r.end(), std::back_inserter(result));
		random_generator->setSeed(time(0));
		r = generateFixedPoints(m ,n-nhome, random_generator);
		std::move (r.begin(), r.end(), std::back_inserter(result));
	}
	return (result);
}

vector<Point*> generateHomeWorkPoints(SimulationConfiguration* sc, unsigned long n, double percentHome) {
	vector<Point*> result;
	Map* m = sc->getMap();
	RandomNumberGenerator* random_generator;
	unsigned int seed = sc->getSeed();
	random_generator = RandomNumberGenerator::instance(seed);

	Geometry* g = m->getBoundary();
	if (g != nullptr) {
		unsigned long nhome = (unsigned long) n * percentHome;
		unsigned int nHomeLocations = sc->getNumHomeLocations();
		unsigned int nPerLocation = nhome / nHomeLocations;
		unsigned int nPerLastLocation = 0;
		if( nHomeLocations * nPerLocation != nhome)
			nPerLastLocation = nhome - (nHomeLocations - 1) * nPerLocation;
		else
			nPerLastLocation = nPerLocation;

		//generate positions for home locations
		for(unsigned int i = 0; i < nHomeLocations; i++) {
			HomeWorkLocation hLoc = sc->getHomeLocation(i);
			if( i != nHomeLocations - 1) {
				generateHomeLocation(m, hLoc, nPerLocation, random_generator, result);
			}
			//last home location
			else {
				generateHomeLocation(m, hLoc, nPerLastLocation, random_generator, result);
			}
		}
		vector<Point*> r = generateFixedPoints(m ,n-nhome, random_generator);
		std::move (r.begin(), r.end(), std::back_inserter(result));
	}
	return (result);
}

void generateHomeLocation(Map* m, HomeWorkLocation hLoc, unsigned int npers, RandomNumberGenerator* random_generator, vector<Point*>& result) {
	vector<pair<const char*, double>> distrPars;
	std::pair<const char*, double> p1 = std::make_pair("mean",hLoc.getX());
	std::pair<const char*, double> p2 = std::make_pair("sd", hLoc.getSdX());
	distrPars.push_back(p1);
	distrPars.push_back(p2);
	Distribution normalX(DistributionType::NORMAL, distrPars);
	vector<pair<const char*, double>> distrPars2;
	p1 = std::make_pair("mean",hLoc.getY());
	p2 = std::make_pair("sd", hLoc.getSdY());
	distrPars2.push_back(p1);
	distrPars2.push_back(p2);
	Distribution normalY(DistributionType::NORMAL, distrPars2);

	double* x1 = random_generator->generateDouble(npers, &normalX);
	double* y1 = random_generator->generateDouble(npers, &normalY);
	unsigned long j = 0, k = 0;
	while (k < npers) {
		if (x1[j] < 1e-15)
			x1[j] = 0.0;
		if (y1[j] < 1e-15)
			y1[j] = 0.0;
		Coordinate c = Coordinate(x1[j], y1[j], 0);
		j++;
		Point* p = m->getGlobalFactory()->createPoint(c);
		if (m->getBoundary()->contains(p)) {
			result.push_back(p);
			k++;
		} else
			m->getGlobalFactory()->destroyGeometry(p);
			// we used all the numbers, generate others
		if (j > npers - 1) {
			delete[] x1;
			delete[] y1;
			x1 = random_generator->generateDouble(npers, &normalX);
			y1 = random_generator->generateDouble(npers, &normalY);
			j = 0;
		}
	}
	delete[] x1;
	delete[] y1;
}

vector<Point*> generateFixedPoints(const Map *m, unsigned long n, RandomNumberGenerator *random_generator) {
	vector<Point*> result;
	Geometry *g = m->getBoundary();
	const Envelope *env = g->getEnvelopeInternal();
	double xmin = env->getMinX();
	double xmax = env->getMaxX();
	double ymin = env->getMinY();
	double ymax = env->getMaxY();

	//generate a pool of numbers to choose from
	double *x1 = random_generator->generateUniformDouble(xmin, xmax, n);
	double *y1 = random_generator->generateUniformDouble(ymin, ymax, n);

	unsigned long i = 0, k = 0;
	while (k < n) {
		if (x1[i] < 1e-15)
			x1[i] = 0.0;
		if (y1[i] < 1e-15)
			y1[i] = 0.0;
		Coordinate c = Coordinate(x1[i], y1[i], 0);
		i++;
		Point *p = m->getGlobalFactory()->createPoint(c);
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
			i = 0;
		}
	}
	delete[] x1;
	delete[] y1;
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

	return result;
}

int getValue(XMLElement* el, const char* name, int default_value) {
	int result = default_value;
	XMLNode* n = getNode(el, name);
	if (n)
		result = atoi(n->ToText()->Value());
	return result;
}

EventType getValue(XMLElement* el, const char* name, EventType default_value) {
	EventType result = default_value;
	XMLNode* n = getNode(el, name);
	if (n) {
		string ss(n->ToText()->Value());
		if( ! (ss.compare("cellID") == 0 || ss.compare("cellIDTA") == 0 )) {
			string msg = string ( "invalid value for " ) + string ( name );
			throw runtime_error(msg);
		}
		else if (!ss.compare("cellID"))
			result = EventType::CELLID;
		else
			result = EventType::CELLIDTA;
	}
	return result;
}

NetworkType getValue(XMLElement* el, const char* name, NetworkType default_value) {
	NetworkType result = default_value;
	XMLNode* n = getNode(el, name);
	if (n) {
		const char* v = n->ToText()->Value();
		if(!strcmp(v, "3G"))
			result = NetworkType::_3G;
		else if(!strcmp(v, "4G"))
			result = NetworkType::_4G;
	}
	return result;
}

double getValue(XMLElement* el, const char* name) {
	double result;
	XMLNode* n = getNode(el, name);
	if (n)
		result = atof(n->ToText()->Value());
	else {
		string msg = string ( "no value for " ) + string ( name );
		throw runtime_error(msg);
	}
	return result;
}

double inverseNormalCDF(const double p, const double mu, const double sigma) {
    if (p < 0.0 || p > 1.0) {
        std::stringstream os;
        os << "Invalid input argument (" << p << "); must be larger than 0 but less than 1.";
        throw std::invalid_argument(os.str());
    }

	if(sigma < 0) {
        std::stringstream os;
        os << "Invalid input argument (" << sigma << "); must be greater than or equal to 0";
        throw std::invalid_argument(os.str());
	}
	if(p == 0)
		return std::numeric_limits<double>::min();
   	if(p == 1)
   		return std::numeric_limits<double>::max();
   	if(sigma == 0)
   		return mu;

   	double r, val;
    const double q = p - 0.5;

    if (std::abs(q) <= .425) {
        r = .180625 - q * q;
        val = q * (((((((r * 2509.0809287301226727 +
                33430.575583588128105) * r + 67265.770927008700853) * r +
                45921.953931549871457) * r + 13731.693765509461125) * r +
                1971.5909503065514427) * r + 133.14166789178437745) * r +
                3.387132872796366608)
            / (((((((r * 5226.495278852854561 +
                28729.085735721942674) * r + 39307.89580009271061) * r +
                21213.794301586595867) * r + 5394.1960214247511077) * r +
                687.1870074920579083) * r + 42.313330701600911252) * r + 1);
    }
    else {
        if (q > 0) {
            r = 1 - p;
        }
        else {
            r = p;
        }
        r = std::sqrt(-std::log(r));
        if (r <= 5) {
            r += -1.6;
            val = (((((((r * 7.7454501427834140764e-4 +
                .0227238449892691845833) * r + .24178072517745061177) *
                r + 1.27045825245236838258) * r +
                3.64784832476320460504) * r + 5.7694972214606914055) *
                r + 4.6303378461565452959) * r +
                1.42343711074968357734)
                / (((((((r *
                    1.05075007164441684324e-9 + 5.475938084995344946e-4) *
                    r + .0151986665636164571966) * r +
                    .14810397642748007459) * r + .68976733498510000455) *
                    r + 1.6763848301838038494) * r +
                    2.05319162663775882187) * r + 1);
        }
        else { /* very close to  0 or 1 */
            r += -5;
            val = (((((((r * 2.01033439929228813265e-7 +
                2.71155556874348757815e-5) * r +
                .0012426609473880784386) * r + .026532189526576123093) *
                r + .29656057182850489123) * r +
                1.7848265399172913358) * r + 5.4637849111641143699) *
                r + 6.6579046435011037772)
                / (((((((r *
                    2.04426310338993978564e-15 + 1.4215117583164458887e-7) *
                    r + 1.8463183175100546818e-5) * r +
                    7.868691311456132591e-4) * r + .0148753612908506148525)
                    * r + .13692988092273580531) * r +
                    .59983220655588793769) * r + 1);
        }
        if (q < 0.0) {
            val = -val;
        }
    }
    return mu + sigma * val;
}

string toString(NetworkType type) {
	string result;
	if(type == NetworkType::_3G)
		result = "3G";
	else if(type == NetworkType::_4G)
		result = "4G";

	return result;
}

}

