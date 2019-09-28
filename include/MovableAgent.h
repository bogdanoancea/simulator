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
 * This class represents an Agent that can move inside the map. This is an abstract class. It is used as a base class for all
 * agents that can move. Currently Person and MobilePhone classes inherits it and are already implemented.
 */
class MovableAgent: public LocatableAgent {
public:

	/**
	 * Constructor of the class.
	 * @param m a pointer to a Map object used by the simulation.
	 * @param id the id of the object.
	 * @param initPosition the initial location on map.
	 * @param clock a pointer to the Clock object used by this simulation.
	 * @param initSpeed the initial speed of the agent. Depending on the derived classes, this value could be read
	 * from a configuration file. For example, for the Person class, this value is specified in the persons.xml configuration file.
	 */
	explicit MovableAgent(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed);

	/**
	 * The default destructor.
	 */
	virtual ~MovableAgent();

	/**
	 * Returns the name of the class.
	 * @return the name of the class.
	 */
	const string getName() const override;

	/**
	 * Builds and returns a human readable string representation of the agent.
	 * @return a human readable string representation of the agent.
	 */
	const string toString() const override;

	/**
	 * A pure virtual method used to move the agent to a new location on the map.
	 * @param type the type of the movement. At this moment there are two values accepted for this parameter:
	 * MovementType::RANDOM_WALK_CLOSED_MAP and MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT.
	 * MovementType::RANDOM_WALK_CLOSED_MAP means that
	 * at each time instant, the direction is generated as a uniformly distributed random number and the step length
	 * is computed multiplying the speed with the time interval set in the simulation configuration file. The agent can only
	 * move inside the map boundary. If a step projects it outside the map, it stops on the boundary.
	 * MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT means that there is a preference in the direction of the movement.
	 * There are two constants defined, SIM_TREND_ANGLE_1 and SIM_TREND_ANGLE_2 (3PI/4 and 5PI/4), and in the first half
	 * of the simulation the direction is generated as a normal distributed random value with the mean equals to SIM_TREND_ANGLE_1 and
	 * sd = 0.1 while during the second half of the simulation it is generated as a normal distributed random value
	 * with the mean equals to SIM_TREND_ANGLE_2 and the same sd. Again, a MovableAgent can only move inside the map boundary.
	 * If a step projects it outside the map, it stops on the boundary.
	 * @return
	 */
	virtual Point* move(MovementType type) = 0;

	/**
	 * Returns the speed of this agent.
	 * @return the speed of this agent.
	 */
	double getSpeed() const;

	/**
	 * Sets the speed of this agent.
	 * @param speed the speed of this agent.
	 */
	void setSpeed(double speed);

private:
	double m_speed;
};

#endif /* SRC_MOVABLEAGENT_H_ */
