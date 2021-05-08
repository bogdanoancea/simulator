/*
 * ProbabilitiesConfiguration.h
 *
 *  Created on: May 8, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PARSERS_PROBABILITIESCONFIGURATION_H_
#define INCLUDE_PARSERS_PROBABILITIESCONFIGURATION_H_

#include <PriorType.h>
#include <string>
using namespace std;

class ProbabilitiesConfiguration {
public:
	ProbabilitiesConfiguration();
	virtual ~ProbabilitiesConfiguration();
	PriorType getPriorType() const;
	void setPrior(PriorType prior);
	string getProbsFilenamePrefix() const;
	void setPrefix(std::string prefix);

private:
	PriorType m_prior;
	string m_probsFileNamePrefix;
};

#endif /* INCLUDE_PARSERS_PROBABILITIESCONFIGURATION_H_ */
