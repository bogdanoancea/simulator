/*
 * ProbabilitiesConfig.h
 *
 *  Created on: Mar 5, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PROBABILITIESCONFIG_H_
#define INCLUDE_PROBABILITIESCONFIG_H_

#include <parsers/ConfigParser.h>
#include <parsers/ProbabilitiesConfiguration.h>
#include <PriorType.h>
#include <string>
#include <vector>





class ProbabilitiesConfigParser: public ConfigParser {
public:
	ProbabilitiesConfigParser(const string& fileName);
	virtual ~ProbabilitiesConfigParser();

	ProbabilitiesConfiguration getConfiguration() const;
	void parse() override;
private:

	ProbabilitiesConfiguration m_configuration;
};

#endif /* INCLUDE_PROBABILITIESCONFIG_H_ */
