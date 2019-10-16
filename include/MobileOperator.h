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
 * MobileOperator.h
 *
 *  Created on: Jul 2, 2019
 *      Author: Bogdan.Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_MOBILEOPERATOR_H_
#define INCLUDE_MOBILEOPERATOR_H_

#include <Agent.h>
#include <string>
#include <fstream>
/**
 * This class represents a Mobile Operator company. Currently a simulation can be run with 1 or 2 mobile operators.
 * A mobile operator own a set of antennas and has a set of mobile phone subscribed.
 */
class MobileOperator: public Agent {
public:
	/**
	 * Constructor of this class. It builds a MobileOperator object with the characteristics provided by user through parameters.
	 * It builds a string representing the name of the file where the coverage area of all the antennas that belong to this MobileOperator
	 * are saved in .csv format and then open this file and writes the header of the file. The name of this file is built
	 * concatenating "AntennaCells_" with the name of the Mobile Operator. The extension of the file is ".csv". A line of this file
	 * contains the antenna id followed by a wkt text representing the coverage area of antenna.
	 * It also builds another string representing the name of a file where the signal quality values for all antennas that belong
	 * to this MobileOperator are saved. The signal quality is computed in the center of each tile of the Grid object set for a simulation. The file is opened
	 * and then it writes the header of the file. The name of this file is built concatenating "SignalQuality_" with the name of
	 * the Mobile Operator. The extension of the file is ".csv". A line of this file contains the antenna id followed by a set of values for
	 * the signal quality computed in the center of each tile of the grid.
	 * @param m a pointer to a Map object where the simulation take place.
	 * @param id the id of the MobileOperator object.
	 * @param clock a pointer to a Clock object used for a simulation.
	 * @param name the name of the Mobile Operator.
	 * @param probMobilePhone represents the probability that a person will have a cell phone at this company.
	 */
	MobileOperator(const Map* m, const unsigned long id, const Clock* clock, const char* name, const double probMobilePhone);

	/**
	 * Default destructor.
	 */
	virtual ~MobileOperator();

	/**
	 * Overrides the same method from the superclass.
	 * @return the name of the class, i.e. "MobileOperator".
	 */
	const string getName() const override;

	/**
	 * Overrides the same method from the superclass. It is used to write the characteristics of the
	 * Mobile Operator to a file or to console.
	 * @return a string that describes the parameters of the MobieOperator.
	 */
	const string toString() const override;

	/**
	 * The name of the Mobile Operator. It should be provided as a parameter to the constructor of the class.
	 * @return The name of the Mobile Operator
	 */
	const string getMNOName() const;


	/**
	 * The probability that a person will have a cell phone at this company. It should be
	 * provided as a parameter to the constructor of the class.
	 *@return the probability that a person will have a cell phone at this company.
	 */
	const double getProbMobilePhone() const;

	/**
	 * @return a file where the coverage area of all the antennas that belong to this MobileOperator
	 * are saved in csv format.
	 */
	ofstream& getAntennaCellsFile() ;

	/**
	 * @return a file where the signal quality/strength/power values for all antennas belonging to this mobile Operator and all tiles of the grid are saved.
	 */
	ofstream& getSignalFile() ;


private:
	const string m_name;
	const double m_probMobilePhone;
	ofstream m_antennaCellsFileName;
	ofstream m_signalMeasureFileName;
};

#endif /* INCLUDE_MOBILEOPERATOR_H_ */
