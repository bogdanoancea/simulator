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
	 * Builds a new Person with the characteristics given as parameters.
	 * @param m a pointer to the Map object where this person move
	 * @param id the id of the person
	 * @param initPosition the initial location of the person on the map
	 * @param clock a pointer to a Clock object used for this simulation
	 * @param initSpeed the inital speed of this person
	 * @param age the age of the person. The age is generated using a uniform or a normal distribution.
	 * @param gender the gender of the person.
	 */
	explicit Person(const Map* m, long id, Point* initPosition, Clock* clock, double initSpeed, int age, Gender gender);

	/**
	 * Destructor
	 */
	virtual ~Person();

	/**
	 * Returns the name of this class
	 * @return the name of this class
	 */
	const string getName() const override;

	/**
	 * Builds a human readable string representation of the person
	 * @return a human readable string representation of the person
	 */
	const string toString() const override;

	/**
	 * Builds a string containing a list with the ids of the mobile devices the this person owns
	 * @return a string containing a list with the ids of the mobile devices the this person owns
	 */
	string dumpDevices();

	/**
	 * returns true if this person has at least a mobile device, false otherwise
	 * @return
	 */
	bool hasDevices();

	/**
	 * Returns the age of the person
	 * @return the age of the person
	 */
	int getAge() const;

	/**
	 * Sets the age of the person
	 * @param age the age of the person
	 */
	void setAge(int age);

	/**
	 * Move the person to another location. Computes the coordinates of the new location and then call setLocation() with this new coordinates.
	 * @param mvType specifies the method used to compute the new position of the person, i.e. how the direction and the length
	 * of the step are computed
	 * @return the new location
	 */
	Point* move(MovementType mvType) override;

	/**
	 * Sets the location of the person on the map
	 * @param pt the location of the person on the map. If the person has mobile devices (phone, tablets) this function call setLocation()
	 * for all mobile devices too.
	 */
	virtual void setLocation(Point* pt) override;

	/**
	 * Add a mobile device to this person. Internally , all mobile devices are kept in an unordered_multimap
	 * as pairs <name of the device class, pointer to the device object>
	 * @param type the name of the device's class
	 * @param agent apointer to the device object
	 */
	void addDevice(string type, Agent* agent) {
		m_idDevices.insert(std::pair<string, Agent*>(type, agent));
	}

	/**
	 * Returns the gender of the person
	 * @return the gender of the person
	 */
	Gender getGender() const;

private:
	int m_age;
	Gender m_gender;
	unordered_multimap<string, Agent*> m_idDevices;
	void randomWalk();

};

#endif /* PERSON_H_ */
