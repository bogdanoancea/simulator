/*
 * Data simulator for mobile phone network events
 *
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#include <Utils.h>
#include <time.h>
#include <RandomNumberGenerator.h>

using namespace geos;
using namespace geos::geom;
using namespace std;

namespace utils {

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

			uniform_real_distribution<double> distributionX(xmin, xmax);
			uniform_real_distribution<double> distributionY(ymin, ymax);
			for (int i = 0; i < n; i++) {
				double x = distributionX(generator);
				double y = distributionY(generator);
				Coordinate c = Coordinate(x, y);
				result.push_back(m->getGlobalFactory()->createPoint(c));
			}
		}
		return (result);
	}

	void printPersonHeader() {
		cout << left << setw(15) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << "Speed" << setw(15) << " Age"
				<< endl;
	}

	void printAntennaHeader() {
		cout << left << setw(15) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(15)
				<< "Max Connections" << setw(20) << "Attenuation Factor" << endl;
	}

	void printPhoneHeader() {
		cout << left << setw(15) << "Phone ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << "Speed" << setw(15) << " Owner id "
				<< endl;
	}

	double* generateNormal2Double(double m1, double sd1, double m2, double sd2, int n, std::mt19937 generator) {
		double* result = new double[n];
		normal_distribution<double> d1 = RandomNumberGenerator::instance()->getNormalDoubleDistribution();
		normal_distribution<double>::param_type p1(m1, sd1);
		normal_distribution<double>::param_type p2(m2, sd2);

		for (int i = 0; i < n; i++) {
			if (i % 2) {
				d1.param(p1);
				result[i] = d1(generator);
			}
			else {
				d1.param(p2);
				result[i] = d1(generator);
			}
		}
		return (result);
	}

	double generateDouble(double min, double max, std::mt19937 generator) {
		double result = 0.0;
		uniform_real_distribution<double>::param_type p(min, max);
		uniform_real_distribution<double> r = RandomNumberGenerator::instance()->getUnifDoubleDistribution();
		r.param(p); //(min, max);

		result = r(generator);
		return (result);
	}
}
