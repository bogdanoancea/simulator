/*
 * SimException.h
 * Data simulator for mobile phone network events
 *  Created on: Aug 30, 2019
 *      Author: bogdanoancea
 */

#ifndef SIMEXCEPTION_H_
#define SIMEXCEPTION_H_

#include <exception>

using namespace std;

class SimException: public exception {

public:
	SimException(const char* msg);
	virtual const char * what () const throw();

	virtual ~SimException();

private:
	const char* msg;
};

#endif /* SIMEXCEPTION_H_ */
