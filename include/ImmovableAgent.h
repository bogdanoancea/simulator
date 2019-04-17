/*
 * Data simulator for mobile phone network events
 *
 * ImmovableAgent.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef IMMOVABLEAGENT_H_
#define IMMOVABLEAGENT_H_

#include "LocatableAgent.h"
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

/*
 *
 */
class ImmovableAgent: public LocatableAgent {
	public:
		ImmovableAgent(Map* m, long id, Point* initialPosition);
		virtual ~ImmovableAgent();

		string toString() override;

		string getName() override {
			return "ImovableAgent";
		}
};

#endif /* IMMOVABLEAGENT_H_ */
