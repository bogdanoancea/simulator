/*
 * DistributionType.h
 *
 *  Created on: Apr 9, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_DISTRIBUTION_H_
#define INCLUDE_DISTRIBUTION_H_

#include <DistributionType.h>
#include <vector>

using namespace std;

class Distribution {
public:
	Distribution() = delete;
	Distribution(DistributionType type, vector<pair<const char*, double>> params);
	virtual ~Distribution();
	double getParam(const char* name);
	void setParams(vector<pair<const char*, double>> params);
	DistributionType getType();

private:
	DistributionType m_type;
	vector<pair<const char*, double>> m_params;

};

#endif /* INCLUDE_DISTRIBUTION_H_ */
