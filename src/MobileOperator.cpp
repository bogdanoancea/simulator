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

MobileOperator::MobileOperator(const Map* m, const unsigned long id, const Clock* clock, const char* name) :
		Agent(m, id, clock), m_name{name} {
}

MobileOperator::~MobileOperator() {
	// TODO Auto-generated destructor stub
}

const string MobileOperator::getName() const {
	return ("MobileOperator");
}

const string MobileOperator::toString() const {
	ostringstream result;
	result << getName() << left << setw(15) << getId();
	return (result.str());
}

const string MobileOperator::getMNOName() const {
	return m_name;
}
