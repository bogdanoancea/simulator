/*
 * PersonConfiguration.h
 *
 *  Created on: May 7, 2021
 *      Author: bogdan
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
