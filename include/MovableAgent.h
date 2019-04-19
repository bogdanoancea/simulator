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
#include <random>

using namespace geos;
using namespace geos::geom;

class MovableAgent: public LocatableAgent {
	public:

		MovableAgent(Map* m, long id, Point* initPosition, double initSpeed);

		virtual ~MovableAgent();

		string getName() override {
			return "MovableAgent";
		}

		string toString() override;


		virtual Point& move(std::mt19937& generator) = 0;

		double getSpeed() const;
		void setSpeed(double speed);

	private:
		double m_speed;
};

#endif /* SRC_MOVABLEAGENT_H_ */
