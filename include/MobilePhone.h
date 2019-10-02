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
 */

#ifndef MOBILEPHONE_H_
#define MOBILEPHONE_H_

#include <HoldableAgent.h>
#include <MobileOperator.h>
#include <Antenna.h>
#include <Clock.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

/**
 * This class represents a mobile phone. A mobile phone is own by a Person and it moves on the map together with its owner.
 * While moving, at every time step it tries to connect to an antenna. The connection event is triggered by setLocation().
 * The connection to antenna is determined by the signal emitted by antennas. A parameter in the simulation configuration file
 * set the criterion used to connect: the power of the signal, the signal strength or the signal quality.
 *
 */
class MobilePhone: public HoldableAgent {
public:
	/**
	 * Builds a new MobilewPhone object with the parameters provided by the user.
	 * @param m a pointer to the Map object where the simulation takes place.
	 * @param id the id of the mobile phone.
	 * @param initPosition the initial location of the phone on the map.
	 * @param holder a pointer to the Agent object that owns this mobile phone.
	 * @param clock a pointer to the Clock object used in this simulation.
	 * @param threshold the minimum power, signal qaulity or signal strength of the field below which the mobile phone cannot connect to an antenna.
	 * @param connType the criterion used for the connection to an antenna: based on the power of the signal or based on the signal quality. It could
	 * take three values: HoldableAgent::CONNECTION_TYPE::USING_POWER, HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY or
	 * HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH.
	 *
	 */
	explicit MobilePhone(const Map* m, const unsigned long id, Point* initPosition, Agent* holder, const Clock* clock, double threshold,
			HoldableAgent::CONNECTION_TYPE connType);

	/**
	 * The default destructor.
	 */
	virtual ~MobilePhone();

	/**
	 * Returns the name of this class.
	 * @return the name of this class.
	 */
	const string getName() const override;
	/**
	 * Returns a human readable string representation of this class useful to output it to a file or console.
	 * @return a human readable string representation of this class.
	 */
	const string toString() const override;

	/**
	 * Makes a step on the map according to an algorithm. The direction and the length of the step is determined by the
	 * mvType parameter and by the Person object who owns this phone.
	 * @param mvType selects the way people and their phones are moving on the map. At this moment only
	 * RANDOM_WALK_CLOSED_MAP and RANDOM_WALK_CLOSED_MAP_WITH_DRIFT are implemented. RANDOM_WALK_CLOSED_MAP means
	 * that at each time instant the direction is generated as a uniformly distributed random value and the
	 * step length is computed multiplying the speed with the time interval set in the simulation configuration file.
	 * If a step projects it outside the map, it stops on the boundary.
	 * MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT means that there is a preference in the direction of the movement.
	 * There are two constants defined, SIM_TREND_ANGLE_1 and SIM_TREND_ANGLE_2 (3PI/4 and 5PI/4), and in the first half
	 * of the simulation the direction is generated as a normal distributed random value with the mean equals to SIM_TREND_ANGLE_1 and
	 * sd = 0.1 while during the second half of the simulation it is generated as a normal distributed random value
	 * with the mean equals to SIM_TREND_ANGLE_2 and the same sd. Again, a MovableAgent can only move inside the map boundary.
	 * If a step projects it outside the map, it stops on the boundary.
	 *
	 *
	 * @return the final location after the movement.
	 */
	Point* move(MovementType mvType) override {
		return (this->getLocation());
	}

	/**
	 * This method is called after the phone moves (together with its owner) to a new location. It tries to connect the mobile phone to an antenna.
	 * The connection method is determined by inspecting the m_connType: using the power of the signal,
	 * using the quality of the signal or using the signal strength. The value of the m_connType is set by the constructor of the class.
	 * If the connection is successfully a pointer to the Antenna object where this mobile phone was connected is stored internally.
	 * @return true if the connection succeeds, false otherwise.
	 */
	bool tryConnect() override;


	/**
	 * Returns the MobileOperator object of this mobile phone. Each MobilePhone should belong to a Mobile Operator.
	 * @return the MobileOperator object of this mobile phone. Each MobilePhone should belong to a Mobile Operator.
	 */
	const MobileOperator* getMobileOperator() const;

	/**
	 * Sets the MobileOperator object which owns this phone.
	 * @param mno the MobileOperator object which owns this phone.
	 */
	void setMobileOperator(MobileOperator* mno);

	/**
	 * Returns the minimum value of the signal strength/power/quality below which the phone cannot use the signal (i.e. the signal is considered noise).
	 * The returned value is interpreted as signal strength, power or quality according to the connection type.
	 * @return the minimum value of the signal strength/power/quality below which the phone cannot use the signal (i.e. the signal is considered noise).
	 */
	double getConnectionThreshold() const;

private:
	//double m_powerThreshold;
	//double m_qualityThreshold;
	double m_threshold;
	Antenna* m_connectedTo;
	HoldableAgent::CONNECTION_TYPE m_connType;
	MobileOperator* m_mno;
};

#endif /* MOBILEPHONE_H_ */
