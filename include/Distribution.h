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
 * DistributionType.h
 *
 *  Created on: Apr 9, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_DISTRIBUTION_H_
#define INCLUDE_DISTRIBUTION_H_

#include <DistributionType.h>
#include <utility>
#include <vector>
#include <TinyXML2.h>
#include <string>

using namespace tinyxml2;


using namespace tinyxml2;
using namespace std;

class Distribution {
public:
	Distribution() = delete;
	Distribution(DistributionType type, vector<pair<const char*, double>> params);
	Distribution(DistributionType type, XMLElement* element);
	virtual ~Distribution();
	double getParam(const char* name);
	vector<pair<const char*, double>>& getParams();
	void setParams(vector<pair<const char*, double>> params);
	DistributionType getType();
	string paramsToString();
	bool hasParam(const char* name) const;
	void setParam(const char* name, double value);

private:
	DistributionType m_type;
	vector<pair<const char*, double>> m_params;
	void parseParams(DistributionType type, XMLElement* element);
	void parseNormalDistributionParams(XMLElement* el);
	void parseTruncatedNormalDistributionParams(XMLElement* el);
	void parseUniformDistributionParams(XMLElement* el);
	void parseLaplaceDistributionParams(XMLElement* el);
	void parseExponentialDistributionParams(XMLElement* el);
	void parseLevyDistributionParams(XMLElement* el);
};

#endif /* INCLUDE_DISTRIBUTION_H_ */
