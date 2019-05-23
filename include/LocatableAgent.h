/*
 * Data simulator for mobile phone network events
 *
 * LocatableAgent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef LOCATABLEAGENT_H_
#define LOCATABLEAGENT_H_

#include <Agent.h>
#include <Clock.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;
using namespace std;

/**
 * his class extends the Agent class and defines an object with a location on a map.
 */
class LocatableAgent: public Agent {
public:

	/**
	 * Constructor of the class. Builds an object that has a location on the map of the simulation
	 * @param m a pointer to a Map object used in this simulation.
	 * @param id the id of the object.
	 * @param initLocation the initial location of the object.
	 * @param clock a pointer to a Clock object used in this simulation.
	 */
	explicit LocatableAgent(Map* m, long id, Point* initLocation, Clock* clock);

	/**
	 * Destructor
	 */
	virtual ~LocatableAgent();

	/**
	 *  Returns the name of this class
	 * @return the name of this class
	 */
	const string getName() override {
		return "LocatableAgent";
	}

	/**
	 * Builds a human readable string representation of this class useful to output it to a file or on the screen.
	 * @return a string representation of this class
	 */
	string toString() const override;

	/**
	 *
	 * @return the location on the map
	 */
	virtual Point* getLocation() const;

	/**
	 * Sets the location of the agent on the map
	 * @param location the location of the agent on the map
	 */
	virtual void setLocation(Point* location);

	/**
	 * Builds a human readable string representation of the location
	 * @return a human readable string representation of the location
	 */
	const string dumpLocation();

private:

	Point* m_location;
};

#endif /* LOCATABLEAGENT_H_ */
