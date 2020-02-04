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
 * MobileOperator.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: Bogdan.Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/MobileOperator.h>
#include <string>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <Constants.h>

using namespace std;

MobileOperator::MobileOperator(const Map* m, const unsigned long id, const Clock* clock, const char* name, const double probMobilePhone) :
		Agent(m, id, clock), m_name { name }, m_probMobilePhone { probMobilePhone } {

	ostringstream cells;
	char sep = Constants::sep;
	cells << "AntennaCells_" << name << ".csv";
	try {
		m_antennaCellsFileName.open(cells.str(), ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening antenna cells output file!" << endl;
	}
	m_antennaCellsFileName << "AntennaId" << sep << "Cell Coordinates" << endl;

	ostringstream quality;
	quality << "SignalMeasure_" << name << ".csv";
	try {
		m_signalMeasureFileName.open(quality.str(), ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening signal quality output file!" << endl;
	}
}

MobileOperator::~MobileOperator() {
	if (m_antennaCellsFileName.is_open()) {
		try {
			m_antennaCellsFileName.close();
		} catch (std::ofstream::failure& e) {
			cerr << "Error closing antenna cells output files!" << endl;
		}
	}
	if (m_signalMeasureFileName.is_open()) {
		try {
			m_signalMeasureFileName.close();
		} catch (std::ofstream::failure& e) {
			cerr << "Error closing signal quality output files!" << endl;
		}
	}
}

const string MobileOperator::toString() const {
	ostringstream result;
	result << left << setw(15) << getId() << setw(15) << getMNOName();
	return (result.str());
}

const string MobileOperator::getMNOName() const {
	return (m_name);
}

const double MobileOperator::getProbMobilePhone() const {
	return (m_probMobilePhone);
}

ofstream& MobileOperator::getAntennaCellsFile() {
	return (m_antennaCellsFileName);
}

ofstream& MobileOperator::getSignalFile() {
	return (m_signalMeasureFileName);
}

const string MobileOperator::getHeader() {
	ostringstream result;
	result << left << setw(15) << "MNO ID" << setw(15) << " Name " << endl;
	return (result.str());
}

