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
 * PersonConfiguration.cpp
 *
 *  Created on: May 7, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <parsers/PersonConfiguration.h>

PersonConfiguration::PersonConfiguration() {
	// TODO Auto-generated constructor stub

}

PersonConfiguration::~PersonConfiguration() {
	// TODO Auto-generated destructor stub
}

int PersonConfiguration::getAge() const {
	return m_age;
}

void PersonConfiguration::setAge(int age) {
	m_age = age;
}

Person::Gender PersonConfiguration::getGender() const {
	return m_gender;
}

void PersonConfiguration::setGender(Person::Gender gender) {
	m_gender = gender;
}

shared_ptr<Distribution> PersonConfiguration::getIntervalBetweenStaysDistribution() const {
	return m_intervalBetweenStaysDistribution;
}

void PersonConfiguration::setIntervalBetweenStaysDistribution(shared_ptr<Distribution> intervalBetweenStaysDistribution) {
	m_intervalBetweenStaysDistribution = intervalBetweenStaysDistribution;
}

double PersonConfiguration::getSpeed() const {
	return m_speed;
}

void PersonConfiguration::setSpeed(double speed) {
	m_speed = speed;
}

shared_ptr<Distribution> PersonConfiguration::getTimeStayDistribution() const {
	return m_timeStayDistribution;
}

void PersonConfiguration::setTimeStayDistribution(shared_ptr<Distribution> timeStayDistribution) {
	m_timeStayDistribution = timeStayDistribution;
}
