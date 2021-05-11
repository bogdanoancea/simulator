/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * ProbabilitiesConfig.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */



#include <Constants.h>
#include <parsers/ProbabilitiesConfigParser.h>
#include <TinyXML2.h>
#include <Utils.h>
#include <cstring>
#include <stdexcept>

using namespace utils;

ProbabilitiesConfigParser::ProbabilitiesConfigParser(const string& fileName): ConfigParser(fileName) {
}

ProbabilitiesConfigParser::~ProbabilitiesConfigParser(){
	// TODO Auto-generated destructor stub
}

void ProbabilitiesConfigParser::parse() {
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
			m_configuration.setPrior(PriorType::NETWORK);
		else if (!strcmp(prior, "uniform"))
			m_configuration.setPrior(PriorType::UNIFORM);
		else if (!strcmp(prior, "register"))
			m_configuration.setPrior(PriorType::REGISTER);
		else
			m_configuration.setPrior(Constants::PRIOR_PROBABILITY);
		m_configuration.setPrefix(getValue(probEl, "prob_file_name_prefix", Constants::PROB_FILE_NAME_PREFIX));
	}
}

ProbabilitiesConfiguration ProbabilitiesConfigParser::getConfiguration() const {
	return m_configuration;
}
