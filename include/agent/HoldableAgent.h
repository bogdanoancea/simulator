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
 * HoldableAgent.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef HOLDABLEAGENT_H_
#define HOLDABLEAGENT_H_



#include <agent/MovableAgent.h>
#include <string>
#include <vector>
class Antenna;
/**
 * This is the superclass for all agents that represent a device that can by held by a person.
 */
class HoldableAgent: public MovableAgent {
public:
	/**
	 * Constructor of the class. It builds an HoldableAgent object with the parameters provided by user.
	 * @param m a pointer to a Map object used for this simulation.
	 * @param id the id of the object.
	 * @param initPosition the initial location on the map of the object.
	 * @param holder a pointer to an Agent that owns this device.
	 * @param clock a pointer to a Clock object used by this simulation.
	 */
	explicit HoldableAgent(const Map* m, const unsigned long id, Point* initPosition, Agent* holder,
			const Clock* clock);

	/**
	 * Copy constructor.
	 * @param h another object of the same type.
	 */
	HoldableAgent(const HoldableAgent &h);

	/**
	 * an enumeration of the modes used by the device to connect to an antenna:
	 * USING_POWER - connects to the antenna that provides the maximum power of the field in the location of the device.
	 * USING_SIGNAL_QUALITY - connects to the antenna that provides the maximum value of the signal quality in the location of the device.
	 * USING_SIGNAL_STRENGTH - connects to the antenna that provides the maximum value of the signal strength in the location of the device.
	 * UNKNOWN - this should by an error.
	 */
	enum CONNECTION_TYPE {
		USING_POWER, USING_SIGNAL_QUALITY, USING_SIGNAL_STRENGTH, UNKNOWN
	};

	/**
	 * Destructor
	 */
	virtual ~HoldableAgent();

	/**
	 * Returns a pointer to an Agent object that owns this device
	 * @return a pointer to an Agent object that owns this device
	 */
	Agent* getHolder() const;

	/**
	 * Sets the owner of this device
	 * @param holder a pointer to the owner of this device
	 */
	void setHolder(Agent* holder);

	/**
	 * Returns a string representation of this class, useful to print it to the console or in a file.
	 * @return a string representation of this class, useful to print it to the console or in a file.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * Called when a device wants to connect to an antenna
	 * @return true if the connection succeeds, false otherwise.
	 */
	virtual bool tryConnect() = 0;

	/**
	 * check if this device is connected to an antenna
	 * @return true if the device is conneted to an antenna, false otherwise.
	 */
	virtual bool isConnected() const;

	/**
	 * Sets the location of this device. After a new location is set, the device tries to connect to an antenna, i.e. the
	 * tryConnect() method is called. Since this is an abstract class and the connection type is unknown it is the responsibility of subclasses
	 * to override this method and provide the correct mode of connection.
	 * @param location a point on the Map of the simulation
	 */
	void setLocation(Point* location) override;

	/**
	 *
	 * @return a vector with pointers to antennas where this device is connected
	 */
	vector<Antenna*> getAntennas() const;

private:
	Agent* m_holder;
	vector<Antenna*> m_antennas;
};

#endif /* HOLDABLEAGENT_H_ */
