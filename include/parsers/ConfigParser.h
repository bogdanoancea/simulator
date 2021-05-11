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
 * ConfigParser.h
 *
 *  Created on: Feb 17, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_CONFIGPARSER_H_
#define INCLUDE_CONFIGPARSER_H_

#include <string>

using namespace std;

/**
 * The base class for the configuration file parsers. All derived classes handle a specific type of
 * configuration file: simulation, persons, antennas, probabilities. These devired classes parse the
 * corresponding configuration file and provided getters to access the values of the configuration
 * parameters. This is an abstract class since it contains one pure virtual function, \code{parse()}
 * that has to be implemented by subclasses.
 */
class ConfigParser {
public:
	/**
	 * Constructor of the class. It takes the filename of the configuration file as a parameter.
	 * @param fileName the name of configuration file to be parsed.
	 */
	ConfigParser(string fileName);

	/**
	 * Returns the name of the configuration file.
	 * @return the name of the configuration file.
	 */
	string getFileName() const;

	/**
	 * Virtual destructor of the class.
	 */
	virtual ~ConfigParser();

	/**
	 * This pure virtual function has to be implemented by specific subclasses. Each subclass
	 * will be specialized to parse one of the configuration file used by this simulation software:
	 * simulation, persons, antennas, probabilities.
	 */
	virtual void parse() = 0;

private:
	string m_fileName;
};

#endif /* INCLUDE_CONFIGPARSER_H_ */
