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
 * PersonConfiguration.h
 *
 *  Created on: May 7, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PARSERS_PERSONCONFIGURATION_H_
#define INCLUDE_PARSERS_PERSONCONFIGURATION_H_

#include <agent/Person.h>
#include <Distribution.h>

/**
 * This class encapsulates the parameters needed to generated a Person object. They are read from the persons configuration file.
 * An object of this type is passed to the constructor of Person.
 */
class PersonConfiguration {
public:

	/**
	 * Default constructor.
	 */
	PersonConfiguration();

	/**
	 * Default destructor.
	 */
	virtual ~PersonConfiguration();

	/**
	 * Returns the age of a person.
	 * @return the age of a person.
	 */
	int getAge() const;

	/**
	 * Sets the age of a person.
	 * @param age the age of a person.
	 */
	void setAge(int age);

	/**
	 * Returns the gender of a person: Person::Gender::MALE or Person::Gender::FEMALE.
	 * @return the gender of a person: Person::Gender::MALE or Person::Gender::FEMALE.
	 */
	Person::Gender getGender() const;

	/**
	 * Sets the gender of a person (Person::Gender::MALE or Person::Gender::FEMALE).
	 * @param gender the gender of a person (Person::Gender::MALE or Person::Gender::FEMALE).
	 */
	void setGender(Person::Gender gender);

	/**
	 * Returns a shared pointer to a Distribution object representing the probability distribution of the time interval between 2 consecutive stops of a person.
	 * The parameters of this distribution are read from the persons configuration file.
	 * @return a shared pointer to a Distribution object representing the probability distribution of the time interval between 2 consecutive stops of a person.
	 * The parameters of this distribution are read from the persons configuration file.
	 */
	shared_ptr<Distribution> getIntervalBetweenStaysDistribution() const;

	/**
	 * Sets the Distribution object representing the probability distribution of the time
	 * interval between 2 consecutive stops of a person.
	 * The parameters of this distribution are read from the persons configuration file.
	 * @param intervalBetweenStaysDistribution * Sets the Distribution object representing the probability distribution of the time
	 * interval between 2 consecutive stops of a person.
	 * The parameters of this distribution are read from the persons configuration file.
	 *
	 */
	void setIntervalBetweenStaysDistribution(shared_ptr<Distribution> intervalBetweenStaysDistribution);

	/**
	 * Returns the speed of a person. This speed is generated according to a probability distribution specified in the persons configuration file.
	 * @return the speed of a person. This speed is generated according to a probability distribution specified in the persons configuration file.
	 */
	double getSpeed() const;

	/**
	 * Sets the speed of a person. This speed is generated according to a probability distribution specified in the persons configuration file.
	 * @param speed the speed of a person. This speed is generated according to a probability distribution specified in the persons configuration file.
	 */
	void setSpeed(double speed);

	/**
     * Returns a shared pointer to a Distribution object representing the probability distribution of the time interval a person stops in the same location.
	 * The parameters of this distribution are read from the persons configuration file.
	 * @return a shared pointer to a Distribution object representing the probability distribution of the time interval a person stops in the same location.
	 * The parameters of this distribution are read from the persons configuration file.
	 */
	shared_ptr<Distribution> getTimeStayDistribution() const;

	/**
	 * Sets the Distribution object representing the probability distribution of the time intervals person stays in the same location.
	 * The parameters of this distribution are read from the persons configuration file.
	 * @param timeStayDistribution the Distribution object representing the probability distribution of the time intervals person stays in the same location.
	 * The parameters of this distribution are read from the persons configuration file.
	 */
	void setTimeStayDistribution(shared_ptr<Distribution> timeStayDistribution);

private:
	double m_speed;
	int m_age;
	Person::Gender m_gender;
	shared_ptr<Distribution> m_timeStayDistribution;
	shared_ptr<Distribution> m_intervalBetweenStaysDistribution;
};

#endif /* INCLUDE_PARSERS_PERSONCONFIGURATION_H_ */
