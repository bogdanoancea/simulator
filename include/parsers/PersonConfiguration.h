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

class PersonConfiguration {
public:
	PersonConfiguration();
	virtual ~PersonConfiguration();
	int getAge() const;
	void setAge(int age);
	Person::Gender getGender() const;
	void setGender(Person::Gender gender);
	shared_ptr<Distribution> getIntervalBetweenStaysDistribution() const;
	void setIntervalBetweenStaysDistribution(shared_ptr<Distribution> intervalBetweenStaysDistribution);
	double getSpeed() const;
	void setSpeed(double speed);
	shared_ptr<Distribution> getTimeStayDistribution() const;
	void setTimeStayDistribution(shared_ptr<Distribution> timeStayDistribution);

private:
	double m_speed;
	int m_age;
	Person::Gender m_gender;
	shared_ptr<Distribution> m_timeStayDistribution;
	shared_ptr<Distribution> m_intervalBetweenStaysDistribution;
};

#endif /* INCLUDE_PARSERS_PERSONCONFIGURATION_H_ */
