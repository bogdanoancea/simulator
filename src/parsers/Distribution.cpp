/*
 * DistributionType.cpp
 *
 *  Created on: Apr 9, 2021
 *      Author: bogdan
 */

#include <parsers/Distribution.h>
#include <string.h>

Distribution::Distribution(DistributionType type, std::vector<pair<const char*, double>> params) {
	m_type = type;
	m_params= params;

}

Distribution::~Distribution() {
	// TODO Auto-generated destructor stub
}

double Distribution::getParam(const char* name) {
	double result = 0;
	for(auto& x: m_params) {
		if(!strcmp(x.first, name))
			result = x.second;
	}
	return result;
}

DistributionType Distribution::getType() {
	return m_type;
}

void Distribution::setParams(vector<pair<const char*, double>> params) {
	m_params = params;
}
