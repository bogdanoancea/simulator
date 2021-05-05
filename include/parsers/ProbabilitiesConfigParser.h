/*
 * ProbabilitiesConfig.h
 *
 *  Created on: Mar 5, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PROBABILITIESCONFIG_H_
#define INCLUDE_PROBABILITIESCONFIG_H_

#include <agent/Person.h>
#include <parsers/ConfigParser.h>
#include <parsers/SimConfigParser.h>
#include <PriorType.h>
#include <string>
#include <vector>





class ProbabilitiesConfig: public ConfigParser {
public:
	ProbabilitiesConfig(const string& fileName);
	virtual ~ProbabilitiesConfig();
	const PriorType getPrior() const;
	const string& getProbsFileNamePrefix() const;
	void parse() override;
private:

	PriorType m_prior;
	string m_probsFileNamePrefix;
};

#endif /* INCLUDE_PROBABILITIESCONFIG_H_ */
