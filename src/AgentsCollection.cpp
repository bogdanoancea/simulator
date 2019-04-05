/*
 * Data simulator for mobile phone network events
 *
 * AgentsCollection.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include "AgentsCollection.h"
#include <algorithm>

AgentsCollection::AgentsCollection() :
		agents { } {
}

AgentsCollection::~AgentsCollection() {
}

void AgentsCollection::addAgent(Agent a) {
	agents.push_back(a);
}

void AgentsCollection::deleteAgent(Agent a) {

	vector<Agent>::iterator position = std::find(agents.begin(), agents.end(),
			a);
	if (position != agents.end()) // == agents.end() means the element was not found
		agents.erase(position);
}
