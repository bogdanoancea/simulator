/*
 * Data simulator for mobile phone network events
 *
 * Tablet.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef TABLET_H_
#define TABLET_H_

#include <HoldableAgent.h>
#include <MovementType.h>

/*
 * This class represents a mobile device, i.e. a tablet that can have a SIM card and can connect to the mobile
 * phone network
 */
class Tablet: public HoldableAgent {
public:
	/**
	 * Builds a new object with the parameters provided by the user
	 * @param m a pointer to a Map object used for simulation.
	 * @param id the id of the tablet
	 * @param initPosition the initial location on map
	 * @param clock a pointer to a Clock object used for simulation.
	 */
	explicit Tablet(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock);

	/**
	 * Destructor
	 */
	virtual ~Tablet();


	/**
	 * Returns the name of this class
	 * @return the name of this class
	 */
	const string getName() const override;

	const string toString() const override;

	/**
	 * This method is called to move the tablet (actually the person who own this tablet move) to another location on the map.
	 * @param type the method used to compute the new location. Could take the following values:
	 * MovementType::RANDOM_WALK, MovementType::RANDOM_WALK_WITH_DRIFT
	 * @return
	 */
	Point* move(MovementType type) override {
		return getLocation();
	}

	/**
	 * Called after the tablet changes location on the map, try to connect to an antenna
	 * @return true if the connection succeeds, false otherwise
	 */
	bool tryConnect() override;

private:

};

#endif /* TABLET_H_ */
