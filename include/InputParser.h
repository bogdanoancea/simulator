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
 * InputParser.h
 *
 *  Created on: May 2, 2019
 *      Author: Bogdan Oancea
 */

#ifndef INPUTPARSER_H_
#define INPUTPARSER_H_

#include <string>
#include <vector>

using namespace std;

/**
 * Utility class used to parse the command line and extract the parameters and their values. An option is passed with
 * a "-" sign in front of it. Example: $simulator -s simulation.xml -p persons
 * Here -s and -p are options and simulation.xml and persons.xml are their corresponding values.
 */
class InputParser {
public:
	/**
	 *  Constructor of the class.
	 * @param argc the number of the arguments from the command line.
	 * @param argv an array with the parameters passed in the command line.
	 */
	InputParser(int &argc, char **argv);

	/**
	 * Returns the value of an option passed as a command line parameter.
	 * @param option an option from the command line.
	 * @return the value of the option passed as a command line parameter.
	 */
	const string& getCmdOption(const string &option) const;

	/**
	 * Checks if an option was passed as a command line parameter.
	 * @param option the option that we are checking.
	 * @return true if the option is present in the command line, false otherwise.
	 */
	bool cmdOptionExists(const string &option) const;
private:
	vector<string> tokens;
};

#endif /* INPUTPARSER_H_ */
