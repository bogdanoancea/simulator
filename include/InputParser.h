/*
 * Data simulator for mobile phone network events
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
 * Utility class used to parse the command line and extract the parameters and their values
 */
class InputParser {
public:
	/**
	 *  Constructor of the class
	 * @param argc The number of the arguments from the command line
	 * @param argv An array with the parameters paassed in the command line
	 */
	InputParser(int &argc, char **argv);
	const string& getCmdOption(const string &option) const;
	bool cmdOptionExists(const string &option) const;
private:
	vector<string> tokens;
};

#endif /* INPUTPARSER_H_ */
