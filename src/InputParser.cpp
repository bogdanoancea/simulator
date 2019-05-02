/*
 * Data simulator for mobile phone network events
 *
 * InputParser.cpp
 *
 *  Created on: May 2, 2019
 *      Author: Bogdan Oancea
 */

#include <InputParser.h>
#include <algorithm>
#include <string>

InputParser::InputParser(int &argc, char **argv) {
	for (int i = 1; i < argc; ++i)
		tokens.push_back(string(argv[i]));
}

const string& InputParser::getCmdOption(const string &option) const {
	vector<string>::const_iterator itr;
	itr = std::find(tokens.begin(), tokens.end(), option);
	if (itr != tokens.end() && ++itr != tokens.end()) {
		return *itr;
	}
	static const string empty_string("");
	return (empty_string);
}

bool InputParser::cmdOptionExists(const string &option) const {
	return find(tokens.begin(), tokens.end(), option) != tokens.end();
}

