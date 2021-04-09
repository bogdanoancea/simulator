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
	Distribution(DistributionType type, map<string, double> params);
	virtual ~Distribution();
	double getParam(string name);
	DistributionType getType();

private:
	DistributionType m_type;
	map<string, double> m_params;

};

#endif /* INCLUDE_PARSERS_DISTRIBUTION_H_ */
