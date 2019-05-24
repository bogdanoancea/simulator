/*
 * Data simulator for mobile phone network events
 *
 * Agent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef AGENT_H
#define AGENT_H

#include <Map.h>
#include <Clock.h>
#include <string>

class Agent {
public:

	/**
	 * Constructor of the class. Agent is the base class for all agents used in the simulator: persons, antennas, devices. Agent is an abstract class,
	 * the users should build specific subclasses
	 * @param m - the Map where the simulation take place.
	 * @param id - the id of this agent, it uniquely identifies the agent
	 * @param clock - the clock used by the simulator, it is the same for all agents
	 */
	Agent(const Map* m, unsigned long id, const Clock* clock);

	/**
	 * Default destructor of the class.
	 */
	virtual ~Agent();

	/**
	 * The equal operator for agents.
	 * @param a the object with which we test the equality
	 * @return true if this object is the equal to a, flase otherwise. Thw objects are considered to be equal if they have the same id.
	 */
	bool operator==(const Agent& a);

	/**
	 * This function is used to get the name of the class. It is a pure virtual function, all subclasses implment it and return the actual name of the class.
	 * @return the name of the class.
	 */
	virtual const string getName() const = 0;

	/**
	 * Builds a string with of the relevant information of the class. It is useful to output on the console or in a file the description of concrete agents.
	 * @return a string representation of the class content. The values of the members are written in this string.
	 */
	virtual const string toString() const = 0;

	/**
	 * Getter that returns a pointer to the map passed to the constructor when the an object was build.
	 * @return a pointer to the Map object that was passed to the constructor. All agents use the same map for a simulation.
	 */
	const Map* getMap() const;

	/**
	 * Sets the Map to be used by this agent during the simulation. It is not advisable to change the map during a simulation.
	 * @param map pointer to a Map object.
	 */
	void setMap(const Map* map);

	/**
	 * Returns a pointer to the Clock object used for simulation. All Agents use the same Clock object for a simulation.
	 * @return
	 */
	const Clock* getClock() const;

	/**
	 * Returns the id of the object.
	 * @return the id of the object.
	 */
	const unsigned long getId() const ;

private:
	const Map* m_map;
	unsigned long m_id;
	const Clock* m_clock;
};

#endif // AGENT_H
