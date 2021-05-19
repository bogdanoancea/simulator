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


/**
 * This class represents a mobile device (tablet). A mobile device is owned by a Person object and it moves on the map
 * together with its owner.
 * While moving, at every time step, when a new location is set, it interacts with surrounding antennas, trying to connect
 * to the antenna that provides the best signal in terms of signal strength/dominance, according to the handover mechanism read from the
 * simulation configuration file. The connection event is triggered by the call of setLocation().
 */
class Tablet: public HoldableAgent {
public:
	/**
	 * Builds a new Tablet object with the parameters provided by the user.
	 * @param m a pointer to a Map object used for simulation.
	 * @param id the id of this object.
	 * @param initPosition the initial location on map.
	 * @param clock a pointer to a Clock object used for simulation..
	 */
	explicit Tablet(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock);

	/**
	 * The default destructor.
	 */
	virtual ~Tablet();


	/**
	 * Builds a string with the most relevant information of the class. It is useful to output the description of the mobile device
	 * to the console or to a file. The parameter <tt>detailed</tt> is currently ignored.
	 * @param detailed is currently ignored.
	 * @return a string object containing the id of the mobile device, the coordinates of the location, the current speed of movement,
	 * the id of the Person object that owns this mobile device and the id of the mobile network operator where this mobile device has a
	 * subscription.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * Makes a step on the map according to the movement strategy set to the Person object which owns this mobile device.
	 * The displacement strategy is set at the Person object creation and currently the following strategies are supported:
	 *  RandomWalkDisplacement, RandomWalkDriftDisplacement, LevyFlightDisplacement and HomeWorkDisplacement.
	 * @return the final location after the displacement. The mobile device moves together with the Person which owns it.
	 */
	Point* move() override {
		return getLocation();
	}

	/**
	 * This method is called after the mobile device moves (together with its owner) to a new location and it tries to connect the mobile device to an antenna.
	 * The connection method is determined by inspecting the \a m_connType member: using the signal dominance or using the signal strength.
	 * The value of the \a m_connType member is set by the constructor of the class.
	 * If the connection is successfully, a pointer to the Antenna object where this mobile phone was connected is stored internally.
	 * First, if the mobile phone is already connected to an antenna, this method checks if the mobile phone is still providing a signal
	 * strong enough for a connection and if not it detaches from that antenna.
	 * Then it selects the antenna providing the best signal (in terms of signal strength or dominance) and tries to connect to it. If the
	 * connection is successful it saves a pointer to that Antenna object. If this antenna reject the connection (for example it reached
	 * its maximum number of simultaneous connections) then a list of all other antennas is build, ordered by the signal strength/dominance.
	 * The mobile phone iterates through this list and tries to connect to an antenna. If one antenna accepts the incoming connection
	 * the method returns true, otherwise it returns false.
	 * When a connection is established, the pointer to the Antenna object is updated accordingly.
	 * @return true if the connection succeeds, false otherwise.
	 */
	bool tryConnect() override;

	/**
	 * Returns a string with the header line of the description of the mobile device returned by the toString() method.
	 * It is a static method, the header line being the same for all mobile devices regardless the owner.
	 * @return a string with the header line of the description of the mobile device returned by the toString() method.
	 */
	static const string getHeader();

private:

};

#endif /* TABLET_H_ */
