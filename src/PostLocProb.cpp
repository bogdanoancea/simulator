/*
 * Copyright (C) 2020 Bogdan Oancea

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
 * PosteriorLocationProbabilities.cpp
 *
 *  Created on: Feb 06, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <PostLocProb.h>
#include <Constants.h>
#include <agent/Agent.h>
#include <agent/MobilePhone.h>
#include <EMField.h>
#include <CSVparser.hpp>
#include <vector>
#include <map>

using namespace std;

PostLocProb::PostLocProb(const Map* m, Clock* clk, AgentsCollection* agents, map<const unsigned long, const string> probFiles) :
		m_map { m }, m_clock { clk }, m_agents { agents }, m_probFilenames { probFiles} {
	// TODO Auto-generated constructor stub

}

PostLocProb::~PostLocProb() {
	// TODO Auto-generated destructor stub
}

void PostLocProb::computeProbabilities() {
	char sep = Constants::sep;
	std::map<unsigned long, vector<AntennaInfo>> data = getAntennaInfo();
	auto itr_mno = m_agents->getAgentListByType(typeid(MobileOperator).name());
	auto itra = m_agents->getAgentListByType(typeid(Antenna).name());

	time_t tt = m_clock->realTime();
	cout << "Computing probabilities started at " << ctime(&tt) << endl;

	auto itrm = m_agents->getAgentListByType(typeid(MobilePhone).name());
	for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
		cout << "Sum signal quality" << " MNO : " << itmno->second->getId() << endl;
		EMField::instance()->sumSignalQuality(m_map, itmno->second->getId());
	}

	ofstream p_file;
	for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
		Agent* mo = itmno->second;
		p_file.open(m_probFilenames[mo->getId()], ios::out);
		writeProbFileHeader(p_file);
		m_clock->reset();
		for (unsigned long t = m_clock->getInitialTime(); t < m_clock->getFinalTime(); t = m_clock->tick()) {
			//iterate over all devices
			for (auto it = itrm.first; it != itrm.second; it++) {
				MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
				if (m->getMobileOperator()->getId() != mo->getId())
					continue;
				p_file << t << sep << m->getId() << sep;

				ostringstream probs;
				vector<double> p = prob(m_map, t, m, data[mo->getId()], itra); // asta e virtuala pura
				for (unsigned long i = 0; i < m_map->getNoTiles() - 1; i++) {
					probs << fixed << setprecision(15) << p[i] << sep;
					//cout << p[i] << ",";
				}
				probs << fixed << setprecision(15) << p[m_map->getNoTiles() - 1];
				//cout << p[map->getGrid()->getNoTiles() - 1] << endl;
				p_file << probs.str() << endl;
			}
		}
		try {
			p_file.close();
		} catch (ofstream::failure& e) {
			cerr << "Error closing probs file!" << endl;
		}
	}
	tt = m_clock->realTime();
	cout << "Computing probabilities ended at " << ctime(&tt) << endl;
}

std::map<unsigned long, vector<AntennaInfo>> PostLocProb::getAntennaInfo() {
	char sep = Constants::sep;
	std::map<unsigned long, vector<AntennaInfo>> data;
	auto itr_mno = m_agents->getAgentListByType(typeid(MobileOperator).name());
	auto itra = m_agents->getAgentListByType(typeid(Antenna).name());

	for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
		MobileOperator* mo = static_cast<MobileOperator*>(itmno->second);
		vector<AntennaInfo> tmp;
		for (auto it = itra.first; it != itra.second; it++) {
			Antenna* a = static_cast<Antenna*>(it->second);
			if (a->getMNO()->getId() == mo->getId()) {
				string fileName = a->getAntennaOutputFileName();
				CSVParser file = CSVParser(fileName, DataType::eFILE, ',', true);
				for (unsigned long i = 0; i < file.rowCount(); i++) {
					Row s = file[i];
					AntennaInfo a(stoul(s[0]), stoul(s[1]), stoul(s[2]), stoul(s[3]), stod(s[4]), stod(s[5]));
					tmp.push_back(a);
				}
			}
			sort(tmp.begin(), tmp.end());
			ofstream antennaInfoFile;
			string name = string("AntennaInfo_MNO_" + mo->getMNOName() + ".csv");
			antennaInfoFile.open(name, ios::out);
			antennaInfoFile << "t,Antenna ID,Event Code,Device ID,x,y, Tile ID" << endl;
			for (AntennaInfo& ai : tmp) {
				antennaInfoFile << ai.toString() << sep << m_map->getTileNo(ai.getX(), ai.getY()) << endl;
			}
			antennaInfoFile.close();
		}
		data.insert(pair<unsigned long, vector<AntennaInfo>>(mo->getId(), tmp));
	}
	return data;
}

void PostLocProb::writeProbFileHeader(ofstream& file) {
	char sep = Constants::sep;
	unsigned long noTiles = m_map->getNoTiles();
	file << "t" << sep << "Phone ID" << sep;
	for (unsigned long i = 0; i < noTiles - 1; i++) {
		file << "Tile" << i << sep;
	}
	file << "Tile" << noTiles - 1 << endl;
}

