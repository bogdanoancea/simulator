/*
 * DistributionType.h
 *
 *  Created on: Apr 9, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_DISTRIBUTION_H_
#define INCLUDE_DISTRIBUTION_H_

#include <DistributionType.h>
#include <utility>
#include <vector>
#include <TinyXML2.h>
#include <string>

using namespace tinyxml2;


using namespace tinyxml2;
using namespace std;

class Distribution {
public:
	Distribution() = delete;
	Distribution(DistributionType type, vector<pair<const char*, double>> params);
	Distribution(DistributionType type, XMLElement* element);
	virtual ~Distribution();
	double getParam(const char* name);
	vector<pair<const char*, double>>& getParams();
	void setParams(vector<pair<const char*, double>> params);
	DistributionType getType();
	string paramsToString();
	bool hasParam(const char* name) const;

private:
	DistributionType m_type;
	vector<pair<const char*, double>> m_params;
	void parseParams(DistributionType type, XMLElement* element);
	void parseNormalDistributionParams(XMLElement* el);
	void parseTruncatedNormalDistributionParams(XMLElement* el);
	void parseUniformDistributionParams(XMLElement* el);
	void parseLaplaceDistributionParams(XMLElement* el);
	void parseExponentialDistributionParams(XMLElement* el);
};

#endif /* INCLUDE_DISTRIBUTION_H_ */
