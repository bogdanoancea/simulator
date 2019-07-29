/*
 * MobileOperator.cpp
 *
 *  Created on: Jul 2, 2019
 *      Author: Bogdan.Oancea
 */

#include <MobileOperator.h>
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
		m_antennaCells.open(cells.str(), ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening antenna cells output file!" << endl;
	}
	m_antennaCells << "AntennaId" << sep << "Cell Coordinates" << endl;

	ostringstream quality;
	quality << "SignalQuality_" << name << ".csv";
	try {
		m_signalQuality.open(quality.str(), ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening signal quality output file!" << endl;
	}
}

MobileOperator::~MobileOperator() {
	if (m_antennaCells.is_open()) {
		try {
			m_antennaCells.close();
		} catch (std::ofstream::failure& e) {
			cerr << "Error closing antenna cells output files!" << endl;
		}
	}

	if (m_signalQuality.is_open()) {
		try {
			m_signalQuality.close();
		} catch (std::ofstream::failure& e) {
			cerr << "Error closing signal quality output files!" << endl;
		}
	}
}

const string MobileOperator::getName() const {
	return ("MobileOperator");
}

const string MobileOperator::toString() const {
	ostringstream result;
	result << left << setw(15) << getId() << setw(15) << getMNOName();
	return (result.str());
}

const string MobileOperator::getMNOName() const {
	return m_name;
}

const double MobileOperator::getProbMobilePhone() const {
	return m_probMobilePhone;
}

ofstream& MobileOperator::getAntennaCellsFile() {
	return m_antennaCells;
}

ofstream& MobileOperator::getSignalQualityFile() {
	return m_signalQuality;
}
