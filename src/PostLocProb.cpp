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
#include <iomanip>
#include <algorithm>
using namespace std;

PostLocProb::PostLocProb(const Map* m, Clock* clk, AgentsCollection* agents, map<const unsigned long, const string> probFiles) :
		m_map { m }, m_clock { clk }, m_agents { agents }, m_probFilenames { probFiles} {
	// TODO Auto-generated constructor stub

}

PostLocProb::~PostLocProb() {
	// TODO Auto-generated destructor stub
}

void PostLocProb::computeProbabilities(std::map<unsigned long, vector<AntennaInfo>> data) {
	char sep = Constants::sep;
	auto itr_mno = m_agents->getAgentListByType(typeid(MobileOperator).name());
	std::pair<um_iterator, um_iterator> itra = m_agents->getAgentListByType(typeid(Antenna).name());

	time_t tt = m_clock->realTime();
	cout << "Computing probabilities started at " << ctime(&tt) << endl;

	auto itrm = m_agents->getAgentListByType(typeid(MobilePhone).name());
	for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
		cout << "Sum signal dominance" << " MNO : " << itmno->second->getId() << endl;
		EMField::instance()->sumSignalDominance(m_map, itmno->second->getId());
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
				vector<double> p = prob(t, m, data[mo->getId()], itra); // asta e virtuala pura
				for (unsigned long i = 0; i < m_map->getNoTiles() - 1; i++) {
					probs << fixed << setprecision(15) << p[i] << sep;
					//cout << p[i] << ",";
				}
				probs << fixed << setprecision(15) << p[m_map->getNoTiles() - 1];
				//cout << p[m_map->getNoTiles() - 1] << endl;
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


void PostLocProb::writeProbFileHeader(ofstream& file) {
	char sep = Constants::sep;
	unsigned long noTiles = m_map->getNoTiles();
	file << "t" << sep << "Phone ID" << sep;
	for (unsigned long i = 0; i < noTiles - 1; i++) {
		file << "Tile" << i << sep;
	}
	file << "Tile" << noTiles - 1 << endl;
}

