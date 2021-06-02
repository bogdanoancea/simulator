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
using namespace std;

/**
 * This is the class represents a probability distribution used for simulations. The parameters of the concrete distributions
 * are read from different configuration files.
 */
class Distribution {
public:
	/**
	 * The default constructor  is deleted. A Distribution object needs to know its type and parameters.
	 */
	Distribution() = delete;

	/**
	 * Constructor of the class. It takes the distribution type and the parameters of the distribution as arguments.
	 * @param type the type of the distribution. It could take one of the following values:
	 * DistributionType::LAPLACE, DistributionType::UNIFORM, DistributionType::NORMAL, DistributionType::TRUNCATED_NORMAL,
	 * DistributionType::LEVY, DistributionType::EXPONENTIAL, DistributionType::BINOMIAL, DistributionType::BERNOULLI
	 * @param params a vector of pairs name-value specifying the name and the values of the parameters of a distribution.
	 */
	Distribution(DistributionType type, vector<pair<const char*, double>> params);

	/**
	 * Constructor of the class. It takes the distribution type and a pointer to an XMLElement to be parsed to extract
	 * the parameters.
	 * @param type thr distribution type: DistributionType::LAPLACE, DistributionType::UNIFORM, DistributionType::NORMAL, DistributionType::TRUNCATED_NORMAL,
	 * DistributionType::LEVY, DistributionType::EXPONENTIAL, DistributionType::BINOMIAL, DistributionType::BERNOULLI.
	 * @param element a pointer to an XMLElement object obtained after parsing a configuration file. The parameters of the distribution
	 * are extracted from this object.
	 */
	Distribution(DistributionType type, XMLElement* element);

	/**
	 * Default destructor.
	 */
	virtual ~Distribution();

	/**
	 * Returns the value of a parameter given its name.
	 * @param name the name of the parameter whose value we want to obtain.
	 * @return the value of a parameter with the specified name.
	 */
	double getParam(const char* name);

	/**
	 * Returns a vector of pairs name-value containing the names and values of the parameters of the distribution.
	 * @return a vector of pairs name-value containing the names and values of the parameters of the distribution.
	 */
	vector<pair<const char*, double>>& getParams();

	/**
	 * Sets the vector of pairs name-value containing the names and values of the parameters of the distribution.
	 * @param params the vector of pairs name-value containing the names and values of the parameters of the distribution.
	 */
	void setParams(vector<pair<const char*, double>> params);

	/**
	 * Returns the distribution type.
	 * @return the distribution type.
	 */
	DistributionType getType();

	/**
	 * Returns a string representation of the distribution. It contains the names and the values of the parameters of the distribution.
	 * @return  a string representation of the distribution. It contains the names and the values of the parameters of the distribution.
	 */
	string paramsToString();

	/**
	 * Checks if this Distribution object has a parameter with the name given as argument and returns true if it has, false otherwise.
	 * @param name the name of a parameter.
	 * @return true if this Distribution object has a parameter with the name given as argument to this method, false otherwise.
	 */
	bool hasParam(const char* name) const;

	/**
	 * Sets a new pair name-value representing a parameter of this distribution. If the Distribution object already has
	 * a parameter with the name given as argument to this method, its sets the value of this parameter to the value
	 * given as the second argument to this function.
	 * @param name the name of a parameter of the distribution.
	 * @param value the value of a parameter of the distribution to be set.
	 */
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
