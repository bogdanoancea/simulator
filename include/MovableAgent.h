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
#include <MovementType.h>
#include <string>

using namespace geos;
using namespace geos::geom;

/**
 * This class represents an agent that can move inside the map
 */
class MovableAgent: public LocatableAgent {
public:

	/**
	 * Builds a new object with the parameters provided by user
	 * @param m a pointer to the map used by the simulation
	 * @param id the id of the object
	 * @param initPosition the initial location on map
	 * @param clock a pointer to the Clock object used by this simulation
	 * @param initSpeed the initial speed of the agent
	 */
	explicit MovableAgent(Map* m, long id, Point* initPosition, Clock* clock, double initSpeed);

	/**
	 * Destructor
	 */
	virtual ~MovableAgent();

	/**
	 * Returns the name of the class
	 * @return the name of the class
	 */
	const string getName() override {
		return ("MovableAgent");
	}

	/**
	 * Builds a human readable string representation of the agent
	 * @return a human readable string representation of the agent
	 */
	string toString() const override;

	/**
	 * A pure virtual method that moves the agent to a new location on the map
	 * @param type the type of the movement. At this moment only
	 * @return
	 */
	virtual Point* move(MovementType type) = 0;

	double getSpeed() const;
	void setSpeed(double speed);

private:
	double m_speed;
};

#endif /* SRC_MOVABLEAGENT_H_ */
