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

#include <agent/Gender.h>
#include <Distribution.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

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
	 * Returns the gender of a person: Gender::MALE or Gender::FEMALE.
	 * @return the gender of a person: Gender::MALE or Gender::FEMALE.
	 */
	Gender getGender() const;

	/**
	 * Sets the gender of a person (Gender::MALE or Gender::FEMALE).
	 * @param gender the gender of a person (Gender::MALE or Gender::FEMALE).
	 */
	void setGender(Gender gender);


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
	 * Returns the location of the person.
	 * @return the location of the person.
	 */
	Point* getLocation() const;

	/**
	 * Sets the location of the person.
	 * @param loc the location of the person.
	 */
	void setLocation(Point* loc);

private:
	double m_speed;
	int m_age;
	Gender m_gender;
	Point* m_location;
};

#endif /* INCLUDE_PARSERS_PERSONCONFIGURATION_H_ */
