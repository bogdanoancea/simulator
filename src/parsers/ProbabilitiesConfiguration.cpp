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
 * ProbabilitiesConfiguration.cpp
 *
 *  Created on: May 8, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
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
