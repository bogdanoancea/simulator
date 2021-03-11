/*
 * ProbabilitiesConfig.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */



#include <Constants.h>
#include <parsers/ProbabilitiesConfig.h>
#include <TinyXML2.h>
#include <Utils.h>
#include <cstring>
#include <stdexcept>

using namespace utils;

ProbabilitiesConfig::ProbabilitiesConfig(const string& fileName): Config(fileName) {
	parse();
}

ProbabilitiesConfig::~ProbabilitiesConfig(){
	// TODO Auto-generated destructor stub
}

void ProbabilitiesConfig::parse() {
	XMLDocument doc;
	string probsFileNamePrefix;
	XMLError err = doc.LoadFile(getFileName().c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for probabilities ");

	XMLElement* probEl = doc.FirstChildElement("probabilities");
	if (!probEl)
		throw std::runtime_error("Syntax error in the configuration file for probabilities ");
	else {
		const char* prior = getValue(probEl, "prior", "UNKNOWN");
		if (!strcmp(prior, "network"))
			m_prior = PriorType::NETWORK;
		else if (!strcmp(prior, "uniform"))
			m_prior = PriorType::UNIFORM;
		else if (!strcmp(prior, "register"))
			m_prior = PriorType::REGISTER;
		else
			m_prior = Constants::PRIOR_PROBABILITY;
		m_probsFileNamePrefix = getValue(probEl, "prob_file_name_prefix", Constants::PROB_FILE_NAME_PREFIX);
	}
}


const PriorType ProbabilitiesConfig::getPrior() const {
	return m_prior;
}

const string& ProbabilitiesConfig::getProbsFileNamePrefix() const {
	return m_probsFileNamePrefix;
}

