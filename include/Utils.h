/*
 * Data simulator for mobile phone network events
 *
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#include <geos/geom/Envelope.h>
#include <geos/geom/Polygon.h>
#include <Map.h>
#include <random>

using namespace geos;
using namespace geos::geom;
using namespace std;


Point* generatePoint(Map* m, default_random_engine generator) {
	Point* result;
	//cout << " enter generate point" << endl;

	Geometry* g = m->getBoundary();
	if (dynamic_cast<Polygon*>(g) != nullptr) {
		Polygon* p = dynamic_cast<Polygon*>(g);
		//cout << "am castuit granita la poligon" << endl;
		const Envelope* env = g->getEnvelopeInternal();
		double xmin = env->getMinX();
		double xmax = env->getMaxX();
		double ymin = env->getMinY();
		double ymax = env->getMaxX();

		uniform_real_distribution<double> distributionX(xmin, xmax);
		double x = distributionX(generator);
		uniform_real_distribution<double> distributionY(ymin, ymax);
		double y = distributionY(generator);
		Coordinate c = Coordinate(x, y);
		result = m->getGlobalFactory()->createPoint(c);
	}
	return (result);
}

int generateAge(int minAge, int maxAge, default_random_engine generator) {

	uniform_int_distribution<int> distributionAge(minAge, maxAge);
	return (distributionAge(generator));
}

