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

#include <LocatableAgent.h>
#include <Clock.h>
#include <random>

using namespace geos;
using namespace geos::geom;

class MovableAgent: public LocatableAgent {
	public:

		explicit MovableAgent(Map* m, long id, Point* initPosition, Clock* clock, double initSpeed);

		virtual ~MovableAgent();

		string getName() override {
			return "MovableAgent";
		}

		string toString() override;


		virtual Point* move() = 0;

		double getSpeed() const;
		void setSpeed(double speed);

	private:
		double m_speed;
};

#endif /* SRC_MOVABLEAGENT_H_ */
