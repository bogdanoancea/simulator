/*
 * ProbabilitiesConfiguration.cpp
 *
 *  Created on: May 8, 2021
 *      Author: bogdan
 */

#include <parsers/ProbabilitiesConfiguration.h>

ProbabilitiesConfiguration::ProbabilitiesConfiguration() {

}

ProbabilitiesConfiguration::~ProbabilitiesConfiguration() {
	// TODO Auto-generated destructor stub
}

PriorType ProbabilitiesConfiguration::getPriorType() const {
	return m_prior;
}
string ProbabilitiesConfiguration::getProbsFilenamePrefix() const {
	return m_probsFileNamePrefix;
}

void ProbabilitiesConfiguration::setPrior(PriorType prior) {
	m_prior = prior;
}

void ProbabilitiesConfiguration::setPrefix(string prefix) {
	m_probsFileNamePrefix = prefix;
}
