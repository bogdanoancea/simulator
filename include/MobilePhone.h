/*
 * Data simulator for mobile phone network events
 *
 * MobilePhone.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef MOBILEPHONE_H_
#define MOBILEPHONE_H_

#include "HoldableAgent.h"
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

/*
 *
 */
class MobilePhone: public HoldableAgent {
	public:
		MobilePhone(Map* m, long id, Point* initPosition, Agent* holder);
		virtual ~MobilePhone();

		string getName() override {
			return "MobilePhone";
		}

		string toString() override;

		Point& move(std::mt19937 *generator) override {
			return this->getLocation();
		}


	private:


};

#endif /* MOBILEPHONE_H_ */
