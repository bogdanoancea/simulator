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
 * This class represents a person that can have 0,1 or 2  mobile phone(s). During the simulation the person move around the map, carrying his/her mobile devices.
 */
class Person: public MovableAgent {
	public:
		enum Gender {
			MALE, FEMALE
		};
		enum AgeDistributions {
			NORMAL, UNIFORM
		};

		/**
		 * Builds a new Person object with the characteristics given as parameters.
		 * @param m a pointer to the Map object where this Person move.
		 * @param id the id of the Person.
		 * @param initPosition the initial location of the person on the map.
		 * @param clock a pointer to a Clock object used for this simulation.
		 * @param initSpeed the initial speed of this person. It is provided in the configuration file.
		 * @param age the age of the person. The age is generated using a uniform or a normal distribution.
		 * @param gender the gender of the person.
		 * @param timeStay the average time of a stop
		 * @param intervalBetweenStays the average time between two consecutive stops.
		 */
		explicit Person(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed, int age,
				Gender gender, unsigned long timeStay, unsigned long intervalBetweenStays);

		/**
		 * The default destructor.
		 */
		virtual ~Person();

		/**
		 * Builds and returns a human readable string representation of the person.
		 * @return a human readable string representation of the person.
		 */
		const string toString() const override;


		static const string getHeader();

		/**
		 * Builds a string containing a list with the ids of the mobile devices that this person owns.
		 * @return a string containing a list with the ids of the mobile devices that this person owns.
		 */
		string dumpDevices();

		//TODO make it private
		/**
		 * returns true if this person has at least a mobile device, false otherwise.
		 * @return
		 */
		bool hasDevices();

		//TODO remove it - never called
		/**
		 * Returns the age of the person.
		 * @return the age of the person.
		 */
		int getAge() const;

		//TODO remove it - never called
		/**
		 * Sets the age of the person.
		 * @param age the age of the person.
		 */
		void setAge(int age);

		/**
		 * Makes a step on the map according to an algorithm. The direction and the length of the step is determined by the
		 * displacement strategy set at the Person creation moment and currently two strategies are supported:
		 * RandomWalkDisplacement and RandomWalkDriftDisplacement.
		 * RandomWalkDisplacement means
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
		virtual Point* move() override;

		/**
		 * \callgraph
		 * Sets the location of the person on the map.
		 * @param pt a pointer to a Point object that represent the location of the person on the map.
		 * If the person has mobile devices (phone, tablets) this function calls setLocation()
		 * for all mobile devices too.
		 */
		virtual void setLocation(Point* pt) override;

		/**
		 * Add a mobile device to this person. Internally, all mobile devices are kept in an unordered_multimap
		 * as pairs <name of the device class, pointer to the device object>
		 * @param type the name of the device's class.
		 * @param agent a pointer to the device object.
		 */
		void addDevice(string type, Agent* agent);

		/**
		 * Sets the displacement algorithm.
		 * @param displace a reference to an implementation of the displacement method. Currently two displacement methods are supported and they are implemented in
		 * RandomWalkDisplacement and RandomWalkDriftDisplacement classes.
		 */
		void setDisplacementMethod(const shared_ptr<Displace>& displace);



	private:
		int m_age;
		Gender m_gender;
		unordered_multimap<string, Agent*> m_idDevices;
		unsigned long m_avgTimeStay;
		unsigned long m_timeStay;
		unsigned long m_avgIntervalBetweenStays;
		unsigned long m_nextStay;
		shared_ptr<Displace> m_displacementMethod;
};

#endif /* PERSON_H_ */
