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

vector<Point*> generatePoints(Map* m, unsigned long n, double percentHome, unsigned seed) {
	vector<Point*> result;
	RandomNumberGenerator* random_generator;
	random_generator = RandomNumberGenerator::instance(seed);

	Geometry* g = m->getBoundary();
	if (dynamic_cast<Polygon*>(g) != nullptr) {
		Polygon* pol = dynamic_cast<Polygon*>(g);
		const Envelope* env = pol->getEnvelopeInternal();
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
			if (pol->contains(p)) {
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
			if (pol->contains(p)) {
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

vector<Point*> generateFixedPoints(Map* m, unsigned long n, unsigned seed) {
	vector<Point*> result;
	RandomNumberGenerator* random_generator;
	random_generator = RandomNumberGenerator::instance(seed);

	Geometry* g = m->getBoundary();
	if (dynamic_cast<Polygon*>(g) != nullptr) {
		Polygon* pol = dynamic_cast<Polygon*>(g);
		const Envelope* env = pol->getEnvelopeInternal();
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
			if (pol->contains(p)) {
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

void printPersonHeader() {
	std::cout << left << std::setw(15) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << "Speed" << setw(15) << " Age"
			<< setw(15) << "Gender" << setw(15) << "Phone(s) ID" << endl;
}

void printAntennaHeader() {
	cout << left << setw(15) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(15)
			<< "Max Connections" << setw(20) << "Attenuation Factor" << setw(15) << "MNO ID" << endl;
}

void printMobileOperatorHeader() {
	cout << left << setw(15) << "MNO ID" << setw(15) << " Name " << endl;
}

void printPhoneHeader() {
	cout << left << setw(15) << "Phone ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Speed " << setw(15) << " Owner id "
			<< setw(15) << "MNO Id" << endl;
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
		result = n->ToText()->Value();;
	return result;
}

int getValue(XMLElement* el, const char* name, int default_value) {
	int result = default_value;
	XMLNode* n = getNode(el, name);
	if (n)
		result = atoi(n->ToText()->Value());
	return result;
}

}
