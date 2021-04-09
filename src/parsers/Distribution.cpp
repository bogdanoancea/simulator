/*
 * DistributionType.cpp
 *
 *  Created on: Apr 9, 2021
 *      Author: bogdan
 */

#include <parsers/Distribution.h>

Distribution::Distribution(DistributionType type, map<string, double> params) {
	m_type = type;
	m_params = params;

}

Distribution::~Distribution() {
	// TODO Auto-generated destructor stub
}

double Distribution::getParam(string name) {
	return m_params[name];
}

DistributionType Distribution::getType() {
	return m_type;
}
