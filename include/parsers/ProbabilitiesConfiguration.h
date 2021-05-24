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
 * ProbabilitiesConfiguration.h
 *
 *  Created on: May 8, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PARSERS_PROBABILITIESCONFIGURATION_H_
#define INCLUDE_PARSERS_PROBABILITIESCONFIGURATION_H_

#include <PriorType.h>
#include <string>
using namespace std;

/**
 * This class encapsulates the parameters read from the probabilities configuration file.
 */
class ProbabilitiesConfiguration {
public:

	/**
	 * Default constructor.
	 */
	ProbabilitiesConfiguration();

	/**
	 * Default destructor.
	 */
	virtual ~ProbabilitiesConfiguration();

	/**
	 * Returns the prior probability type (PriorType::UNIFORM, PriorType::REGISTER, PriorType::NETWORK).
	 * @return
	 */
	PriorType getPriorType() const;

	/**
	 * Sets the the prior probability type (PriorType::UNIFORM, PriorType::REGISTER, PriorType::NETWORK).
	 * @param prior
	 */
	void setPrior(PriorType prior);

	/**
	 * Returns the prefix of the output file where the location probabilities are saved. The file name is composed concatenating the
	 * prefix with "_" and MNO name read from the simulation configuration files. The files extension is ".csv".
	 *
	 * @return the prefix of the output file where the location probabilities are saved. The file name is composed concatenating the
	 * prefix with "_" and MNO name read from the simulation configuration files. The files extension is ".csv".
	 */
	string getProbsFilenamePrefix() const;

	/**
	 * Sets the prefix of the output file where the location probabilities are saved. The file name is composed concatenating the
	 * prefix with "_" and MNO name read from the simulation configuration files. The files extension is ".csv".
	 * @param prefix the prefix of the output file where the location probabilities are saved. The file name is composed concatenating the
	 * prefix with "_" and MNO name read from the simulation configuration files. The files extension is ".csv".
	 */
	void setPrefix(std::string prefix);

private:
	PriorType m_prior;
	string m_probsFileNamePrefix;
};

#endif /* INCLUDE_PARSERS_PROBABILITIESCONFIGURATION_H_ */
