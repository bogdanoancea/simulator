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
 * PosteriorLocationProbabilities.h
 *
 *  Created on: Feb 06, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_POSTLOCPROB_H_
#define INCLUDE_POSTLOCPROB_H_

#include <AntennaInfo.h>
#include <agent/AgentsCollection.h>
#include <map/Map.h>
#include <agent/MobilePhone.h>
#include <AntennaInfo.h>
#include <Clock.h>
#include <map>

using namespace std;

class PostLocProb {
public:
	PostLocProb(const Map* m, Clock* clk, AgentsCollection* agents, map<const unsigned long, const string> probFiles);

	virtual ~PostLocProb();

	virtual void computeProbabilities();

	virtual vector<double> prob(unsigned long t, MobilePhone* m, vector<AntennaInfo>& data, pair<um_iterator, um_iterator> it) = 0;

protected:
	const Map* m_map;
	Clock* m_clock;
	AgentsCollection* m_agents;
	map<const unsigned long, const string> m_probFilenames;

	/**
	 * At the end of a simulation this method merges all the events saved by individual antennas in a single data structure.
	 * @return a map of <MNO_ID, vector<AntennInfo>> where for each MNO identified by its ID has
	 * vector of all events saved by all antennas belonging to that MNO. This map is needed for computation of location probabilities.
	 */

	map<unsigned long, vector<AntennaInfo>> getAntennaInfo();
	void writeProbFileHeader(ofstream& f);

};

#endif /* SRC_POSTERIORLOCATIONPROBABILITIES_H_ */
