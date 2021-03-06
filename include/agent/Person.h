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
 * Person.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <agent/MovableAgent.h>
#include <Displace.h>
#include <memory>
#include <string>
#include <unordered_map>

using namespace geos;
using namespace geos::geom;

/**
 * This class represents a person that can have 0, 1 or 2 mobile phone(s). During the simulation, the person move
 * around the map, carrying his/her mobile device(s). At every time step, a new location is computed according to a movement
 * strategy.
 */
class Person: public MovableAgent {
public:
	enum Gender {
		MALE, FEMALE
	};
	/**
	 * Builds a new Person object with the characteristics given as parameters.
	 * @param m a pointer to the Map object where this Person move.
	 * @param id the id of the Person object.
	 * @param initPosition the initial location of the person on the map.
	 * @param clock a pointer to a Clock object used for this simulation.
	 * @param initSpeed the initial speed of this person. It is provided in the configuration file.
	 * @param age the age of the person. The age is generated using a uniform or a truncated normal distribution.
	 * The actual type of the age distribution is specified in the persons configuration file. Each person will have
	 * an age generated according to this distribution.
	 * @param gender the gender of the person.
	 * @param timeStayDistribution the Distribution of the time interval the person stays in the same location. For certain types
	 * of movement patterns (random move, random move with drift, Levy flights the movement consists in a sequence of time intervals
	 * when the person stays in the same position and time intervals when the person move to a new position. The time interval when the
	 * person stays in the same position is a random value generated according the distribution given as parameter.
	 * @param intervalBetweenStaysDistribution the Distribution of the time interval between two consecutive stops of the person.
	 */
	explicit Person(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed, int age, Gender gender,
			shared_ptr<Distribution> timeStayDistribution, shared_ptr<Distribution> intervalBetweenStaysDistribution);

	/**
	 * The default destructor.
	 */
	virtual ~Person();

	/**
	 * Builds a string with the most relevant information of the class. It is useful to output the description of a person
	 * to the console or to a file. Currently, the value of the <tt>detailed</tt> parameter is ignored.
	 * @param detailed the value of this parameter is ignored.
	 * @return a string object containing the id, coordinates of the location of the person, speed of movement, age, gender, and the
	 * ids of the devices held by this person.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * Builds a string with the column names of the values included in the result of the toString() method.
	 * @return a string with the column names of the values included in the result of the toString() method.
	 */
	static const string getHeader();

	/**
	 * Builds a string containing a list with the ids of the mobile devices that this person owns.
	 * @return a string containing a list with the ids of the mobile devices that this person owns.
	 */
	string dumpDevices();

	//TODO make it private
	/**
	 * Returns true if this person has at least one mobile device, false otherwise.
	 * @return true if this person has at least one mobile device, false otherwise.
	 */
	bool hasDevices();

	//TODO remove it - never called
	/**
	 * Returns the age of the person.
	 * @return the age of the person.
	 */
	int getAge() const;

	/**
	 * This method is called at every time step. It computes a new location on the map, according to a movement pattern.
	 * The direction and the length of the step is determined by the displacement strategy set at the Person creation
	 * and currently four strategies are supported  RandomWalkDisplacement, RandomWalkDriftDisplacement,
	 * LevyFlightDisplacement and HomeWorkDisplacement.
	 * @return the final location after the displacement.
	 */
	virtual Point* move() override;

	/**
	 * Sets the location of the person on the map.
	 * @param pt a pointer to a Point object that represent the location of the person on the map.
	 * If the person has mobile devices (phones, tablets) this function calls setLocation()
	 * for all mobile devices too.
	 */
	virtual void setLocation(Point* pt) override;

	/**
	 * Adds a mobile device to this person. Internally, all mobile devices are kept in an unordered_multimap
	 * as pairs composed by the device class name and a pointer to the device object.
	 * @param type the device class name
	 * @param agent a pointer to the device object.
	 */
	void addDevice(string type, Agent* agent);

	/**
	 * Sets the displacement pattern of the person.
	 * @param displace a reference to a shared_ptr<Displace> pointer to the displacement pattern object. Displace
	 * is an abstract class, and this method is actually called with a concrete implementation of it.
	 */
	void setDisplacementMethod(const shared_ptr<Displace>& displace);

	/**
	 * Sets the home location of the person when a home-work displacement pattern is used for simulation. For other displacement
	 * patterns this method is not used.
	 * @param hl a pointer to a Point object representing the home location of the person.
	 */
	void setHomeLocation(Point* hl);

	/**
	 * Returns the home location of the person in case a home-work displacement pattern is used for simulation and nullptr
	 * otherwise.
	 * @return the home location of the person in case a home-work displacement pattern is used for simulation and nullptr
	 * otherwise.
	 */
	Point* getHomeLocation() const;

	/**
	 * Sets the work location of the person when a home-work displacement pattern is used for simulation. For other displacement
	 * patterns this method is not used.
	 * @param wl a pointer to a Point object representing the work location of the person.
	 * */
	void setWorkLocation(Point* wl);

	/**
	 * Returns the work location of the person in case a home-work displacement pattern is used for simulation and nullptr
	 * otherwise.
	 * @return the work location of the person in case a home-work displacement pattern is used for simulation and nullptr
	 * otherwise.
	 */
	Point* getWorkLocation() const;

	/**
	 * Sets the anchor location of the person when a home-work displacement pattern is used for simulation. For other displacement
	 * patterns this method is not used. An anchor point is optional in the home-work displacement pattern and is a place visited by a person
	 * in his/her trip from the work location to the home location. Once reached this point, the person will stay here for a period of
	 * time and then goes toward the home location. Information about the anchor points are read from the simulation configuration file. The
	 * details about how to anchor point locations are generated, the trajectory work location - anchor location - home location can be
	 * found in the documentation of the HomeWorkScenario.
	 * @param al a pointer to a Point object representing the anchor location of the person.
	 * */
	void setAnchorLocation(Point* al);

	/**
	 * Returns the anchor location of the person when a home-work displacement pattern is used for simulation. For other displacement
	 * patterns this method return nullptr. An anchor point is optional in the home-work displacement pattern and is a place
	 * visited by a person in his/her trip from the work location to the home location. Once reached this point, the person will stay
	 * here for a period of time and then goes toward the home location. Information about the anchor points are read from the
	 * simulation configuration file. The details about how to anchor point locations are generated, the trajectory
	 * work location - anchor location - home location can be found in the documentation of the HomeWorkScenario.
	 *
	 * @return a pointer to a Point object representing the anchor location of the person.
	 */
	Point* getAnchorLocation() const;

	/**
	 * Returns true if this person is a person who follows a home-work trajectory. Even for the home-work simulation scenario, a number
	 * of persons will move randomly on the map. For there persons, the method will return false. The number of such persons who
	 * move randomly is given (as a proportion) in the simulation configuration file.
	 * @return true if the person follow a home-work trajectory, false otherwise.
	 */
	bool isHomePerson() const;

	/**
	 * Returns a shared pointer to a Distribution object representing the probability distribution of the time interval between two consecutive
	 * stops of a person. This time interval is generated as a random value using a distribution specified by the user in the simulation
	 * configuration file. Currently, only the exponential distribution is accepted.
	 * @return a reference to a shared_ptr<Distribution> object representing the probability distribution of the time interval between two consecutive
	 * stops of a person.
	 */
	const shared_ptr<Distribution>& getIntervalBetweenStaysDistribution() const;

	/**
	 * Sets the probability distribution of the time interval between two consecutive stops of a person.
	 * @param intervalBetweenStaysDistribution a reference to a shared pointer to a Distribution object representing the probability distribution of the
	 * time interval between two consecutive stops of a person. This time interval is generated as a random value using a distribution
	 * specified by the user in the simulation configuration file. Currently, only the exponential distribution is accepted. This method is currently
	 * used to set this pointer to nullptr for a home-work displacement scenario, for the persons that move randomly on the map. For the other
	 *  displacement patterns this distribution is set by the constructor of the Person class.
	 */
	void setIntervalBetweenStaysDistribution(const shared_ptr<Distribution> &intervalBetweenStaysDistribution);

	/**
	 * Returns a shared pointer to a Distribution object representing the probability distribution of the time interval a person stays
	 * in the same location. This time interval is generated as a random value using a distribution specified by the user in the simulation
	 * configuration file. Currently, only the normal and uniform distributions are accepted.
	 * @return a reference to ashared_ptr<Distribution> object representing the probability distribution of the time interval a person stops in the same location.
	 */
	const shared_ptr<Distribution>& getTimeStayDistribution() const;

	/**
	 * Sets the probability distribution of the  time interval a person stops in the same location.
	 * @param timeStayDistribution a shared pointer to a Distribution object representing the probability distribution of the
	 * time interval a person stops in the same location. This time interval is generated as a random value using a distribution
	 * specified by the user in the simulation configuration file. Currently, only the normal and uniform distributions are accepted. This method is currently
	 * used to set this pointer to nullptr for a home-work displacement scenario, for the persons that move randomly on the map. For the other
	 *  displacement patterns this distribution is set by the constructor of the Person class.
	 */
	void setTimeStayDistribution(const shared_ptr<Distribution> &timeStayDistribution);

private:
	int m_age;
	Gender m_gender;
	unordered_multimap<string, Agent*> m_idDevices;
	shared_ptr<Distribution> m_timeStayDistribution;
	unsigned long m_timeStay;
	shared_ptr<Distribution> m_intervalBetweenStaysDistribution;
	unsigned long m_nextStay;
	shared_ptr<Displace> m_displacementMethod;
	Point* m_homeLocation;
	Point* m_workLocation;
	Point* m_anchorLocation;
};

#endif /* PERSON_H_ */
