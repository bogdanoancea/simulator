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
 * This is a class that represents an agent that can have a location on map but it cannot move. The only subclass of it is Antenna.
 */
class ImmovableAgent: public LocatableAgent {
public:

	/**
	 * Constructor of the class. Build an ImmovableAgent object with the parameters provided by the user.
	 * @param m a pointer to a Map object used in this simulation.
	 * @param id the id of this object.
	 * @param initialPosition the initial location on map.
	 * @param clock a pointer to a Clock object used in this simulation.
	 */
	explicit ImmovableAgent(const Map* m, const unsigned long id, Point* initialPosition,
			const Clock* clock);

	/**
	 * Default destructor.
	 */
	virtual ~ImmovableAgent();

	/**
	 * Builds a string representation of this class.
	 * @return a string representation of this class. It is used to write details of the ImmovableAgent objects in a file or on console.
	 */
	const string toString() const override;


	/**
	 * Returns the name of this class.
	 * @return the name of this class.
	 */
	const string getName() const override;
};

#endif /* IMMOVABLEAGENT_H_ */
