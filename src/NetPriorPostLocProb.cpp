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
 * NetPriorPostLocnProb.h
 *
 *  Created on: Feb 06, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/AgentsCollection.h>
#include <agent/Antenna.h>
#include <agent/MobilePhone.h>
#include <AntennaInfo.h>
#include <events/EventCode.h>
#include <geos/geom/Coordinate.h>
#include <map/Map.h>
#include <NetPriorPostLocProb.h>
#include <iostream>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

NetPriorPostLocProb::NetPriorPostLocProb(const Map* m, Clock* clk, AgentsCollection* agents, map<const unsigned long, const string> probFiles) :
		PostLocProb(m, clk, agents, probFiles) {
	// TODO Auto-generated constructor stub

}

NetPriorPostLocProb::~NetPriorPostLocProb() {
	// TODO Auto-generated destructor stub
}


vector<double> NetPriorPostLocProb::prob(unsigned long t, MobilePhone* m, vector<AntennaInfo>& data, pair<um_iterator, um_iterator> it) {
	vector<double> result;
	// take the mobile phone and see which is the antenna connected to

	vector<AntennaInfo>::iterator ai;
	bool found = false;
	for (vector<AntennaInfo>::iterator i = data.begin(); i != data.end(); i++) {
		ai = i;
		if (ai->getTime() == t && ai->getDeviceId() == m->getId()
				&& (ai->getEventCode() == static_cast<int>(EventCode::ATTACH_DEVICE)
						|| ai->getEventCode() == static_cast<int>(EventCode::ALREADY_ATTACHED_DEVICE))) {
			found = true;

			break;
		}
	}

	double sum = 0.0;
	for (unsigned long tileIndex = 0; tileIndex < m_map->getNoTiles(); tileIndex++) {
		double lh = 0.0;
		if (found) {
			Coordinate c = m_map->getTileCenter(tileIndex);
			unsigned long antennaId = ai->getAntennaId();
			Antenna* a = nullptr;
			for (auto itr = it.first; itr != it.second; itr++) {
				a = dynamic_cast<Antenna*>(itr->second);
				if (a->getId() == antennaId) {
					break;
				}
			}
			c.z = 0; //TODO tile elevation!
			if (a != nullptr) {
				lh = a->computeSignalDominance(c);
				sum += lh;
			}
		}
		result.push_back(lh);
	}
	for (auto& i : result) {
		if (sum != 0.0) {
			i /= sum;
		}
	}
	return (result);
}

