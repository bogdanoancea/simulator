/*
 * PersonConfiguration.cpp
 *
 *  Created on: May 7, 2021
 *      Author: bogdan
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
