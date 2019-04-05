/*
 * MovableAgent.h
 *
 *  Created on: Apr 5, 2019
 *      Author: bogdan
 */

#ifndef MOVABLEAGENT_H_
#define MOVABLEAGENT_H_

#include <LocatableAgent.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

class MovableAgent: public LocatableAgent {
	public:

		MovableAgent();

		virtual ~MovableAgent();

		Point move();
		double getSpeed() const;
		void setSpeed(double speed);

	private:
		double m_speed;
};

#endif /* SRC_MOVABLEAGENT_H_ */
