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
/*
 *
 */
class InputParser {
	public:
		InputParser(int &argc, char **argv);
		const string& getCmdOption(const string &option) const;
		bool cmdOptionExists(const string &option) const;
	private:
		vector<string> tokens;
};


#endif /* INPUTPARSER_H_ */
