/*
 * Data simulator for mobile phone network events
 *
 * MovableAgent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */



#ifndef MOVABLEAGENT_H_
#define MOVABLEAGENT_H_

#include "LocatableAgent.h"
#include "World.h"
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

class MovableAgent: public LocatableAgent {
	public:

		MovableAgent(World& w, long id, Point& initPosition);

		virtual ~MovableAgent();

		Point move();
		double getSpeed() const;
		void setSpeed(double speed);

	private:
		double m_speed;
};

#endif /* SRC_MOVABLEAGENT_H_ */
