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

#include <agent/Agent.h>
#include <fstream>
#include <string>

/**
 * This class represents a Mobile Operator company. Currently a simulation can be run with 1 or 2 mobile operators.
 * A mobile operator owns a set of antennas and has a set of mobile phone subscribed. It has a name read from the simulation configuration
 * file and two ofstream members representing the files where the cell boundaries and signal strength/dominance are saved for all
 * antennas belonging to this mobile operator. It also has a member representing the probability of a person to have a mobile phone subscribed
 * to this mobile network operator. The value of this probability is read from the simulation configuration file.
 */
class MobileOperator: public Agent {
public:
	/**
	 * Constructor of this class. It builds a MobileOperator object with the characteristics provided by user through the
	 * parameters of the constructor.
	 * In the process of creating the object, the constructor method builds a string representing the name of the file where the
	 * coverage area of all the antennas (cell boundary)
	 * that belong to this MobileOperator are saved in .csv format, then open this file and writes the header of the file.
	 * The name of this file is built by concatenating "AntennaCells_" with the name of the Mobile Operator. A line of this file
	 * contains the antenna id followed by a <tt>wkt</tt> text representing the coverage area of antenna.
	 * It also builds another string representing the name of a file where the signal strength/dominance values for all antennas that belong
	 * to this MobileOperator are saved. The signal strength/dominance is computed in the center of each tile of the Grid
	 * object set for a simulation. The file is opened and then it writes the header of the file. The name of this file is built by
	 * concatenating "SignalMeasure_" with the name of the Mobile Operator.  A line of this file contains the antenna id followed
	 * by a set of values for the signal strength/dominance computed in the center of each tile of the grid.
	 * @param m a pointer to a Map object of the simulation.
	 * @param id the id of the MobileOperator object.
	 * @param clock a pointer to a Clock object used for a simulation.
	 * @param name the name of the Mobile Operator.
	 * @param probMobilePhone represents the probability for a person to have a mobile phone subscribed to this company.
	 * @param outputDir a string representing the name of the directory where the output files are written.
	 */
	MobileOperator(const Map* m, const unsigned long id, const Clock* clock, const char* name, const double probMobilePhone, const string& outputDir);

	/**
	 * Default destructor.
	 */
	virtual ~MobileOperator();

	/**
	 * Overrides the same method from the superclass. It is used to write the characteristics of the
	 * Mobile Operator to a file or to console.
	 * @param detailed this parameter has no effect for the MobileOperator class.
	 * @return a string that describes the parameters of the MobieOperator. This string contains the id of the MobileOperator
	 * object and its name.
	 */
	const string toString(bool detailed = false) const override;

	/**
	 * Returns the header of the string representation of MobileOperator objects.
	 * @return the header of the string representation of MobileOperator objects.
	 */
	static const string getHeader();

	/**
	 * The name of the Mobile Operator. This name is read from the simulation configuration file and it is provided as a parameter
	 * to the constructor of the class.
	 * @return the name of the Mobile Operator
	 */
	const string getMNOName() const;

	/**
	 * The probability for a person to have a mobile phone subscribed to this company. It is
	 * provided as a parameter to the constructor of the class.
	 *@return the probability for a person to have a mobile phone subscribed to this company.
	 */
	const double getProbMobilePhone() const;


	/**
	 * Returns the file where the coverage area of all the antennas that belong to this MobileOperator are saved in .csv format.
	 * @return an ofstream object representing the file where the coverage area of all the antennas that belong to this
	 * MobileOperator are saved in .csv format.
	 */
	ofstream& getAntennaCellsFile();

	/**
	 * Returns the file where the signal strength/dominance values are saved in .csv format.
	 * @return an ofstream representing the file where the signal strength/dominance values for all antennas
	 * belonging to this mobile operator and all tiles of the grid are saved.
	 */
	ofstream& getSignalFile();

	/**
	 * Writes the header of the file where the signal strength/dominance is saved.
	 */
	void writeSignalFileHeader();

	/**
	 * Returns the name of the folder where the output files are saved.
	 * @return the name of the folder where the output files are saved.
	 */
	const string& getOutputDir() const;

private:
	const string m_name;
	const double m_probMobilePhone;
	ofstream m_antennaCellsFileName;
	ofstream m_signalMeasureFileName;
	const string& m_outputDir;
};

#endif /* INCLUDE_MOBILEOPERATOR_H_ */
