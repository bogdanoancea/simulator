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
 */

#ifndef TABLET_H_
#define TABLET_H_

#include <HoldableAgent.h>
#include <MovementType.h>

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
	 * Returns the name of this class.
	 * @return the name of this class.
	 */
	const string getName() const override;

	/**
	 * Builds a human readable representation of this class.
	 * @return a human readable representation of this class.
	 */
	const string toString() const override;

	/**
	 * This method is called to move the tablet (actually the person who own this tablet move) to another location on the map.
	 * @param type the method used to compute the new location. For details see MovableAgent::move() and Person::move().
	 * @return a pointer to a Point object representing the new location on the map.
	 */
	Point* move(MovementType type) override {
		return getLocation();
	}

	/**
	 * Called after the tablet changes location on the map, tries to connect to an antenna.
	 * @return true if the connection succeeds, false otherwise.
	 */
	bool tryConnect() override;

private:

};

#endif /* TABLET_H_ */
