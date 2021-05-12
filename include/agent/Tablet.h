/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * Tablet.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef TABLET_H_
#define TABLET_H_

#include <agent/HoldableAgent.h>
#include <string>


/*
 * This class represents a mobile device, i.e. a tablet that can have a SIM card and can connect to the mobile
 * phone network. Actually it is not used but it is intended for futures developments.
 */
class Tablet: public HoldableAgent {
public:
	/**
	 * Builds a new Tablet object with the parameters provided by the user.
	 * @param m a pointer to a Map object used for simulation.
	 * @param id the id of the tablet.
	 * @param initPosition the initial location on map.
	 * @param clock a pointer to a Clock object used for simulation..
	 */
	explicit Tablet(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock);

	/**
	 * The default destructor.
	 */
	virtual ~Tablet();


	/**
	 * Builds a human readable representation of this class.
	 * @return a human readable representation of this class.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * Makes a step on the map according to an algorithm. The direction and the length of the step is determined by the
	 * displacement strategy and by the Person object who owns this phone. The displacement strategy is set at the Person
	 * object creation and currently two strategies are supported: RandomWalkDisplacement and RandomWalkDriftDisplacement.
	 *  RandomWalkDisplacement means
	 * that at each time instant the direction is generated as a uniformly distributed random value and the
	 * step length is computed multiplying the speed with the time interval set in the simulation configuration file.
	 * If a step projects it outside the map, it stops on the boundary.
	 * RandomWalkDriftDisplacement means that there is a preference in the direction of the movement.
	 * There are two constants defined, SIM_TREND_ANGLE_1 and SIM_TREND_ANGLE_2 (3PI/4 and 5PI/4), and in the first half
	 * of the simulation the direction is generated as a normal distributed random value with the mean equals to SIM_TREND_ANGLE_1 and
	 * sd = 0.1 while during the second half of the simulation it is generated as a normal distributed random value
	 * with the mean equals to SIM_TREND_ANGLE_2 and the same sd. Again, any kind of MovableAgent can only move inside the map boundary.
	 * If a step projects it outside the map, it stops on the boundary.
	 * @return the final location after the displacement.
	 */
	Point* move() override {
		return getLocation();
	}

	/**
	 * Called after the tablet changes location on the map, tries to connect to an antenna.
	 * @return true if the connection succeeds, false otherwise.
	 */
	bool tryConnect() override;

		static const string getHeader();

private:

};

#endif /* TABLET_H_ */
