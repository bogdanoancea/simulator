/*
 * MobileOperator.h
 *
 *  Created on: Jul 2, 2019
 *      Author: Bogdan.Oancea
 */

#ifndef INCLUDE_MOBILEOPERATOR_H_
#define INCLUDE_MOBILEOPERATOR_H_

#include <Agent.h>
#include <string>
#include <fstream>

class MobileOperator: public Agent {
public:
	MobileOperator(const Map* m, const unsigned long id, const Clock* clock, const char* name, const double probMobilePhone);

	virtual ~MobileOperator();

	/**
	 * Overrides the same method from the superclass.
	 * @return the name of the class, i.e. "MobileOperator"
	 */
	const string getName() const override;

	/**
	 * Overrides the same method from the superclass. It is used to write the characteristics of the MNO in a file or console.
	 * @return a string that describes the parameters of the Antenna.
	 */
	const string toString() const override;

	const string getMNOName() const;
	const double getProbMobilePhone() const;
	ofstream& getAntennaCellsFile() ;

private:
	const string m_name;
	const double m_probMobilePhone;
	ofstream m_antennaCells;
};

#endif /* INCLUDE_MOBILEOPERATOR_H_ */
