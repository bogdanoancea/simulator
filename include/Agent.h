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
	Agent(Map* m, unsigned long id, Clock* clock);

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

	virtual string getName() = 0;
	virtual string toString() = 0;

	Map* getMap() const;
	void setMap(Map* map);

	Clock* getClock() const;

	unsigned long getId() const {
		return m_id;
	}

	void setId(unsigned long id) {
		m_id = id;
	}

private:
	Map* m_map;
	unsigned long m_id;
	Clock* m_clock;
};

#endif // AGENT_H
