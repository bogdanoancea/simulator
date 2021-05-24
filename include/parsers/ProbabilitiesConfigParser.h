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
 * ProbabilitiesConfig.h
 *
 *  Created on: Mar 5, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PROBABILITIESCONFIG_H_
#define INCLUDE_PROBABILITIESCONFIG_H_

#include <parsers/ConfigParser.h>
#include <parsers/ProbabilitiesConfiguration.h>
#include <PriorType.h>
#include <string>
#include <vector>



/**
 * This class parses the probabilities configuration file, passed with "-pb" option in the command line.
 * It reads the method used to compute the location probabilities for each device and the prior distribution.
 * These location probabilities are computed here only for a preliminary study, they are actually computed with another software tool.
 */

class ProbabilitiesConfigParser: public ConfigParser {
public:

	/**
	 * constructor of the class. It takes the name of the probabilities configuration file as parameter and stores it internally.
	 * @param fileName the name of the probabilities configuration file passed with "-pb" option in the command line.
	 */
	ProbabilitiesConfigParser(const string& fileName);

	/**
	 * default destructor.
	 */
	virtual ~ProbabilitiesConfigParser();

	/**
	 * Returns a ProbabilitiesConfiguration object which encapsulates the parameters need to computed the location probabilities.
	 * @return a ProbabilitiesConfiguration object which encapsulates the parameters need to computed the location probabilities.
	 */
	ProbabilitiesConfiguration getConfiguration() const;

	/**
	 * It parses the probabilities configuration file and builds a ProbabilitiesConfiguration object.
	 */
	void parse() override;
private:

	ProbabilitiesConfiguration m_configuration;
};

#endif /* INCLUDE_PROBABILITIESCONFIG_H_ */
