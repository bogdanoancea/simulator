/*
 * Data simulator for mobile phone network events
 *
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#include <geos/geom/Coordinate.h>
#include <geos/geom/Envelope.h>
#include <geos/geom/GeometryFactory.h>
#include <Map.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>
#include <iomanip>
#include <iostream>

#include <geos/geom/Polygon.h>

namespace utils {
	using namespace geos;
	using namespace geos::geom;
	using namespace std;

	vector<Point*> generatePoints(Map* m, int n) {
		vector<Point*> result;

		Geometry* g = m->getBoundary();
		if (dynamic_cast<Polygon*>(g) != nullptr) {
			Polygon* pol = dynamic_cast<Polygon*>(g);
			const Envelope* env = pol->getEnvelopeInternal();
			double xmin = env->getMinX();
			double xmax = env->getMaxX();
			double ymin = env->getMinY();
			double ymax = env->getMaxX();

			//generate a pool of numbers to choose from
			double* x = RandomNumberGenerator::instance()->generateUniformDouble(xmin, xmax, n);
			double* y = RandomNumberGenerator::instance()->generateUniformDouble(ymin, ymax, n);
			int i = 0, k = 0;
			while (k < n) {
				Coordinate c = Coordinate(x[i], y[i]);
				i++;
				Point* p = m->getGlobalFactory()->createPoint(c);
				if(pol->contains(p)) {
					result.push_back(p);
					k++;
				}
				else
					m->getGlobalFactory()->destroyGeometry(p);

				// we used all the numbers, generate others
				if (i == n) {
					delete[] x;
					delete[] y;
					x = RandomNumberGenerator::instance()->generateUniformDouble(xmin, xmax, n-k+10);
					y = RandomNumberGenerator::instance()->generateUniformDouble(ymin, ymax, n-k+10);
					i = 0;
				}
			}
			delete[] x;
			delete[] y;
		}
		return (result);
	}

	void printPersonHeader() {
		std::cout << left << std::setw(15) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << "Speed" << setw(15)
				<< " Age" << setw(15)<< "Gender" << endl;
	}

	void printAntennaHeader() {
		cout << left << setw(15) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(15)
				<< "Max Connections" << setw(20) << "Attenuation Factor" << endl;
	}

	void printPhoneHeader() {
		cout << left << setw(15) << "Phone ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << "Speed" << setw(15) << " Owner id "
				<< endl;
	}

	XMLNode* getNode(XMLElement* el, const char* name) {
		XMLNode* n = el->FirstChildElement(name)->FirstChild();
		if (!n)
			throw std::runtime_error("Syntax error in the configuration file for antennas ");
		return (n);
	}

	XMLElement* getFirstChildElement(XMLElement* el, const char* name) {
		XMLElement* n = el->FirstChildElement(name);
		if (!n)
			throw std::runtime_error("Syntax error in the configuration file for antennas ");
		return (n);
	}
}
