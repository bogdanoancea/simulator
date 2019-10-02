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
 * InputParser.cpp
 *
 *  Created on: May 2, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <InputParser.h>
#include <algorithm>
#include <string>

using namespace std;

InputParser::InputParser(int &argc, char **argv) {
	for (int i = 1; i < argc; ++i)
		tokens.push_back(string(argv[i]));
}

const string& InputParser::getCmdOption(const string &option) const {
	vector<string>::const_iterator itr;
	itr = find(tokens.begin(), tokens.end(), option);
	if (itr != tokens.end() && ++itr != tokens.end()) {
		return *itr;
	}
	static const string empty_string("");
	return (empty_string);
}

bool InputParser::cmdOptionExists(const string &option) const {
	return find(tokens.begin(), tokens.end(), option) != tokens.end();
}

