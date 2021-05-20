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
 * MobilePhone.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef MOBILEPHONE_H_
#define MOBILEPHONE_H_

#include <agent/HoldableAgent.h>
#include <agent/MobileOperator.h>
#include <string>

using namespace geos;
using namespace geos::geom;

/**
 * This class represents a mobile phone. A mobile phone is owned by a Person object and it moves on the map
 * together with its owner.
 * While moving, at every time step, when a new location is set, it interacts with surrounding antennas, trying to connect
 * to the antenna that provides the best signal in terms of signal strength/dominance, according to the handover mechanism read from the
 * simulation configuration file. The connection event is triggered by the call of setLocation().
 */
class MobilePhone: public HoldableAgent {
public:
	/**
	 * Builds a new MobilePhone object with the parameters provided by the user.
	 * @param m a pointer to the Map object where the simulation takes place.
	 * @param id the id of the MobilePhone object.
	 * @param initPosition the initial location of the mobile phone on the map.
	 * @param holder a pointer to the Agent object that owns this mobile phone.
	 * @param clock a pointer to the Clock object used in this simulation.
	 * @param threshold the minimum signal dominance or signal strength of the field below which the mobile phone cannot
	 * connect to an antenna.
	 * @param connType the type of the handover mechanism: based on the signal strength or signal dominance. It could
	 * take one of the following values: HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_DOMINANCE or
	 * HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH.
	 */
	explicit MobilePhone(const Map* m, const unsigned long id, Point* initPosition, Agent* holder, const Clock* clock, double threshold, HoldableAgent::CONNECTION_TYPE connType);

	/**
	 * The default destructor of the class.
	 */
	virtual ~MobilePhone();

	/**
	 * Builds a string with the most relevant information of the class. It is useful to output the description of the mobile phone
	 * to the console or to a file. The parameter\param{detailed} is currently ignored.
	 * @param detailed is currently ignored.
	 * @return a string object containing the id of the mobile phone, the coordinates of the location, the current speed of movement,
	 * the id of the Person object that owns this mobile phone and the id of the mobile network operator where this mobile phone has a
	 * subscription.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * Returns a string with the header line of the description of the mobile phone returned by the toString() method.
	 * It is a static method, the header line being the same for all mobile phones regardless the owner.
	 * @return a string with the header line of the description of the mobile phone returned by the toString() method.
	 */
	static const string getHeader();

	/**
	 * Makes a step on the map according to the movement strategy set to the Person object which owns this mobile phone.
	 * The displacement strategy is set at the Person object creation and currently the following strategies are supported:
	 *  RandomWalkDisplacement, RandomWalkDriftDisplacement, LevyFlightDisplacement and HomeWorkDisplacement.
	 * @return the final location after the displacement. The mobile phone moves together with the Person which owns it.
	 */
	Point* move() override {
		return (this->getLocation());
	}

	/**
	 * This method is called after the mobile phone moves (together with its owner) to a new location.
	 * It tries to connect the mobile phone to an antenna.
	 * The connection method is determined by inspecting the \a m_connType member: using the signal dominance or using the signal strength.
	 * The value of the \a m_connType member is set by the constructor of the class.
	 * If the connection is successfully, a pointer to the Antenna object where this mobile phone was connected is stored internally.
	 * First, if the mobile phone is already connected to an antenna, this method checks if the mobile phone is still providing a signal
	 * strong enough for a connection and if not it detaches from that antenna.
	 * Then it selects the antenna providing the best signal (in terms of signal strength or dominance) and tries to connect to it. If the
	 * connection is successful it saves a pointer to that Antenna object. If this antenna reject the connection( for example it reached
	 * its maximum number of simultaneous connections) then a list of all other antennas is build, ordered by the signal strength/dominance.
	 * The mobile phone iterates through this list and tries to connect to an antenna. If one antenna accepts the incoming connection
	 * the method returns true, otherwise it returns false.
	 *  When a connection is established, the pointer to the Antenna object is updated accordingly.
	 * @return true if the connection succeeds, false otherwise.
	 */
	bool tryConnect() override;

	/**
	 * Returns the MobileOperator object that owns this mobile phone. Each mobile phone should belong to a Mobile Operator.
	 * @return the MobileOperator object that owns this mobile phone. Each mobile phone should belong to a Mobile Operator.
	 */
	const MobileOperator* getMobileOperator() const;

	/**
	 * Sets the MobileOperator object which owns this mobile phone.
	 * @param mno a pointer to the MobileOperator object which owns this mobile phone.
	 */
	void setMobileOperator(MobileOperator* mno);

private:
	double m_threshold;
	Antenna* m_connectedTo;
	HoldableAgent::CONNECTION_TYPE m_connType;
	MobileOperator* m_mno;
};

#endif /* MOBILEPHONE_H_ */
