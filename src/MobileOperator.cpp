/*
 * MobileOperator.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: Bogdan.Oancea
 */

#include <MobileOperator.h>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>

using namespace std;

MobileOperator::MobileOperator(const Map* m, const unsigned long id, const Clock* clock, const char* name, const double probMobilePhone) :
		Agent(m, id, clock), m_name{name}, m_probMobilePhone{probMobilePhone} {
}

MobileOperator::~MobileOperator() {
	// TODO Auto-generated destructor stub
}

const string MobileOperator::getName() const {
	return ("MobileOperator");
}

const string MobileOperator::toString() const {
	ostringstream result;
	result << left << setw(15) << getId() << setw(15) << getMNOName();
	return (result.str());
}

const string MobileOperator::getMNOName() const {
	return m_name;
}

const double MobileOperator::getProbMobilePhone() const {
	return m_probMobilePhone;
}
