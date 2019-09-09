/*
 * Data simulator for mobile phone network events
 *
 * Person.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <MovableAgent.h>
#include <geos/geom/Point.h>
#include <string>
#include <unordered_map>

using namespace geos;
using namespace geos::geom;

/**
 * This class represents a person that can have a mobile phone.
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
	 */
	explicit Person(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double initSpeed, int age, Gender gender, unsigned long timeStay, unsigned long intervalBetweenStays);

	/**
	 * The default destructor.
	 */
	virtual ~Person();

	/**
	 * Returns the name of this class.
	 * @return the name of this class.
	 */
	const string getName() const override;

	/**
	 * Builds and returns a human readable string representation of the person.
	 * @return a human readable string representation of the person.
	 */
	const string toString() const override;

	/**
	 * Builds a string containing a list with the ids of the mobile devices that this person owns.
	 * @return a string containing a list with the ids of the mobile devices that this person owns.
	 */
	string dumpDevices();

	/**
	 * returns true if this person has at least a mobile device, false otherwise.
	 * @return
	 */
	bool hasDevices();

	/**
	 * Returns the age of the person.
	 * @return the age of the person.
	 */
	int getAge() const;

	/**
	 * Sets the age of the person.
	 * @param age the age of the person.
	 */
	void setAge(int age);

	/**
	 * Move the person to another location. Computes the coordinates of the new location and then call setLocation() with this new coordinates.
	 * @param mvType specifies the method used to compute the new position of the person, i.e. how the direction and the length
	 * of the step are computed. It can have the following values:
	 * RANDOM_WALK_CLOSED_MAP - the agent moves randomly inside the map boundary. The direction is generated
	 * as a random value at each time step and the step length is computed multiplying the speed with the time interval.
	 * RANDOM_WALK_CLOSED_MAP_WITH_DRIFT:  the agent moves in a preferential direction. There are two constants defining these directions:
	 * SIM_TREND_ANGLE_1 and SIM_TREND_ANGLE_2 (3PI/4 and 5PI/4). The actual direction is generated as a normally distributed random
	 * value with means equals to these constants and sad = 0.1.
	 * In both cases, a Person makes several steps then a stop. The average time length of a stop is given is the simulation configuration
	 * file while the actual values are generated as normal distributed random values with the mean read from the configuration file and
	 * and sd = 20% from mean. The average time interval elapsed between two stop is read from the simulation configuration file and
	 * the actual values are generated as exponential distributed random values.
	 * @return a pointer to a Point object that represents the new location.
	 */
	Point* move(MovementType mvType) override;

	/**
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
	 * Returns the gender of the person.
	 * @return the gender of the person.
	 */
	Gender getGender() const;


	/**
	 * The average time interval a person stay in the same location. It is given in the simulation.xml configuration file.
	 * @return the average time a person stay in the same location.
	 */
	unsigned long getAvgTimeStay() const;

	/**
	 * The average time interval between two stops. It is given in the simulation.xml configuration file.
	 * @return The average time interval between two stops.
	 */
	unsigned long getAvgIntervalBetweenStays() const;

private:
	int m_age;
	Gender m_gender;
	unordered_multimap<string, Agent*> m_idDevices;
	bool m_changeDirection;
	unsigned long m_avgTimeStay;
	unsigned long m_timeStay;
	unsigned long m_avgIntervalBetweenStays;
	unsigned long m_nextStay;
	void randomWalkClosedMap();
	void randomWalkClosedMapDrift();
	Point* generateNewLocation(double theta);
	void setNewLocation(Point* p, bool changeDirection);
};

#endif /* PERSON_H_ */
