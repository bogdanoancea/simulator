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

#ifndef INCLUDE_NETPRIORPOSTLOCPROB_H_
#define INCLUDE_NETPRIORPOSTLOCPROB_H_

#include <PostLocProb.h>

/**
 * This class is part of the Strategy design pattern used to compute the posterior location probabilities. It implements
 * the computation of the posterior location probabilities starting from the prior probabilities given by the network,
 * overriding the prob() method from its superclass, \class PostLocProb.
 */
class NetPriorPostLocProb: public PostLocProb {
public:
	/**
	 * Constructor of the class. It sets the members according to the values given as parameters.
	 * @param m a pointer to the Map object of the simulation.
	 * @param clk a pointer to the Clock object of the simulation.
	 * @param agents a pointer to the AgentsCollection object.
	 * @param probFiles the name of the files where the posterior probabilities are saved (one file per MNO).
	 */
	NetPriorPostLocProb(const Map* m, Clock* clk, AgentsCollection* agents, map<const unsigned long, const string> probFiles);

	/**
	 * Default destructor
	 */
	virtual ~NetPriorPostLocProb();

	/**
	 * Implements the computation of the posterior location probabilities with the prior probabilities given by the network.
	 * @param t the time instant for which the probabilities are computed.
	 * @param m a pointer to a MobilePhone object for which the probabilities are computed.
	 * @param data a vector with the network events.
	 * @param it an iterator the the antenna's list.
	 * @return a vector with the posterior location probabilities for each time instant.
	 */
	virtual vector<double> prob(unsigned long t, MobilePhone* m, vector<AntennaInfo>& data, pair<um_iterator, um_iterator> it) override;
};

#endif /* INCLUDE_NETPRIORPOSTLOCPROB_H_ */
