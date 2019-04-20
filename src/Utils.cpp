/*
 * Data simulator for mobile phone network events
 *
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#include <RandomNumberGenerator.h>
#include <Utils.h>

using namespace geos;
using namespace geos::geom;
using namespace std;

namespace utils {

	vector<Point*> generatePoints(Map* m, int n) {
		vector<Point*> result;

		Geometry* g = m->getBoundary();
		if (dynamic_cast<Polygon*>(g) != nullptr) {
			Polygon* p = dynamic_cast<Polygon*>(g);
			const Envelope* env = p->getEnvelopeInternal();
			double xmin = env->getMinX();
			double xmax = env->getMaxX();
			double ymin = env->getMinY();
			double ymax = env->getMaxX();

			double* x = new double[n];
			double *y = new double[n];
			x = RandomNumberGenerator::instance()->generateDouble(xmin, xmax, n);
			y = RandomNumberGenerator::instance()->generateDouble(ymin, ymax, n);
			for (int i = 0; i < n; i++) {
				Coordinate c = Coordinate(x[i], y[i]);
				result.push_back(m->getGlobalFactory()->createPoint(c));
			}
			delete x;
			delete y;
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


}
