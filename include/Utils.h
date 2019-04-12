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


vector<Point*> generatePoints(Map* m, std::mt19937 generator, int n) {
	vector<Point*> result;


	Geometry* g = m->getBoundary();
	if (dynamic_cast<Polygon*>(g) != nullptr) {
		Polygon* p = dynamic_cast<Polygon*>(g);
		const Envelope* env = p->getEnvelopeInternal();
		double xmin = env->getMinX();
		double xmax = env->getMaxX();
		double ymin = env->getMinY();
		double ymax = env->getMaxX();

		static uniform_real_distribution<double> distributionX(xmin, xmax);
		static uniform_real_distribution<double> distributionY(ymin, ymax);
		for (int i = 0; i < n; i++) {
			double x = distributionX(generator);
			double y = distributionY(generator);
			Coordinate c = Coordinate(x, y);
			result.push_back(m->getGlobalFactory()->createPoint(c));
		}
	}
	return (result);
}

//int generateAge(int minAge, int maxAge, std::mt19937 generator) {
//
//	uniform_int_distribution<int> distributionAge(minAge, maxAge);
//	return (distributionAge(generator));
//}

