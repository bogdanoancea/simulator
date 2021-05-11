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
