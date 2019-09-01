/*
 * SimException.cpp
 * Data simulator for mobile phone network events
 *  Created on: Aug 30, 2019
 *      Author: bogdanoancea
 */

#include "SimException.h"

using namespace std;

SimException::SimException(const char* msg) :
		msg(msg){
}

const char * SimException::what() const throw(){
	return msg;
}

SimException::~SimException() {
// TODO Auto-generated destructor stub
}

