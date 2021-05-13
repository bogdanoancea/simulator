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
 * This is the superclass for all agents that represent a device (\code{MobilePhone} or \code{Tablet}), can by held by a person
 * and can interact with the network. The interaction is triggered every time the location of this agent is set.
 *
 */
class HoldableAgent: public MovableAgent {
public:
	/**
	 * Constructor of the class. It builds a \code{HoldableAgent} object with the parameters provided by user.
	 * @param m a pointer to a \code{Map} object used for this simulation.
	 * @param id the id of the object.
	 * @param initPosition a pointer to a \code{Point} object representing the initial location on the map of this object.
	 * @param holder a pointer to an \code{Agent} object that owns (and carries) this device.
	 * @param clock a pointer to a \code{Clock} object used by this simulation.
	 */
	explicit HoldableAgent(const Map* m, const unsigned long id, Point* initPosition, Agent* holder, const Clock* clock);


	/**
	 * An enumeration of the modes used by the device to connect to an antenna:
	 * \li USING_POWER - connects to the antenna that provides the maximum power of the field in the location of the device.
	 * \li USING_SIGNAL_QUALITY - connects to the antenna that provides the maximum value of the signal quality in the location of the device.
	 * \li USING_SIGNAL_STRENGTH - connects to the antenna that provides the maximum value of the signal strength in the location of the device.
	 * \li UNKNOWN - this should by an error.
	 * The actual handover mechanism is set in the simulation configuration file.
	 */
	enum CONNECTION_TYPE {
		USING_POWER, USING_SIGNAL_DOMINANCE, USING_SIGNAL_STRENGTH, UNKNOWN
	};

	/**
	 * Destructor of the class.
	 */
	virtual ~HoldableAgent();

	/**
	 * Returns a pointer to an \code{Agent} object that owns this device. Actually, the holder can be  a \code{Person} object.
	 * @return a pointer to an \code{Agent} object that owns this device.
	 */
	Agent* getHolder() const;

	/**
	 * Sets the owner agent of this device.
	 * @param holder a pointer to an \code{Agent} object which is the owner of this device.
	 */
	void setHolder(Agent* holder);


	/**
	 * Builds a \code{string} with the most relevant or all information of the class. It is useful to output the description of concrete agents
	 * to the console or to a file. Depending on the value of the \param{detailed} parameter, the string can contain only the values of the
	 * the most important members (\code{detailed = false}) or all members of the class.
	 * @param detailed if true, the string will contain the values of all members/parameters of the agent, otherwise only the most
	 * important ones are written to the output string. Each derived class will decide which are the most important members.
	 * @return a \code{string} object containing the most important or all parameters/members of the agent.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * A pure virtual method which is called when a device interacts with an antenna. This function is called every time when \code{setLocation()} was called,
	 * i.e. the location of the device on the map was updated. If the connection was successful, i.e. an antenna accepted and connected this device,
	 * the id of the antenna is saved in an internal list.
	 * @return true if the connection succeeds, false otherwise.
	 */
	virtual bool tryConnect() = 0;

	/**
	 * Check if this device is connected to an antenna.
	 * @return true if the device is connected to an antenna, false otherwise.
	 */
	virtual bool isConnected() const;

	/**
	 * Sets the location of this device. After a new location is set, the device tries to connect to an antenna, i.e. the
	 * tryConnect() method is called. Since this is an abstract class and the connection type is unknown it is the responsibility of subclasses
	 * to override the \code{tryConnect()} method and provide the correct mode of connection.
	 * @param location a pointer to a \code{Point} object that represent the new position on the map.
	 */
	void setLocation(Point* location) override;


private:
	Agent* m_holder;
	vector<Antenna*> m_antennas;
};

#endif /* HOLDABLEAGENT_H_ */
