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
#include <Clock.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

/**
 * This is a class that represents an agent that can have a localion on map but it cannot move. The only subclass of it is Antenna.
 */
class ImmovableAgent: public LocatableAgent {
public:

	/**
	 * Constructor of the class. Build an object of ImmovableAgent type with the parameters provided by the user.
	 * @param m a pointer to a map object used in this simulation.
	 * @param id the id of this object
	 * @param initialPosition the initial location on map
	 * @param clock a pointer to a Clock object used in this simulation
	 */
	explicit ImmovableAgent(Map* m, long id, Point* initialPosition,
			Clock* clock);

	/**
	 * Destructor
	 */
	virtual ~ImmovableAgent();

	/**
	 * Builds a string representation of this class
	 * @return a string representation of this class
	 */
	string toString() override;


	/**
	 * Returns the name of this class
	 * @return the name of this class
	 */
	const string getName() override {
		return "ImovableAgent";
	}
};

#endif /* IMMOVABLEAGENT_H_ */
