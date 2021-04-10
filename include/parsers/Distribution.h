/*
 * DistributionType.h
 *
 *  Created on: Apr 9, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PARSERS_DISTRIBUTION_H_
#define INCLUDE_PARSERS_DISTRIBUTION_H_

#include <DistributionType.h>
#include <map>

using namespace std;

class Distribution {
public:
	Distribution() = delete;
	Distribution(DistributionType type, std::map<const char*, double> &params);
	virtual ~Distribution();
	double getParam(const char* name);
	DistributionType getType();

private:
	DistributionType m_type;
	map<const char*, double> m_params;

};

#endif /* INCLUDE_PARSERS_DISTRIBUTION_H_ */
