/*
 * Data simulator for mobile phone network events
 *
 * World.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */


#include <World.h>
#include <iostream>

using namespace std;
//ctor
World::World() {
}

//dtor
World::~World() {
}

void World::runSimulation() {
	cout << "Hello from our mobile phone simulator!" << endl;

}

unsigned int World::getCurrentTime() {
	return m_clock->getCurrentTime();
}

//AgentsCollection* World::getAgents() const {
//	return m_agents;
//}
//
//void World::setAgents(AgentsCollection* agents) {
//	m_agents = agents;
//}

Clock* World::getClock() const {
	return m_clock;
}

void World::setClock(Clock* clock) {
	m_clock = clock;
}

Map* World::getMap() const {
	return m_map;
}

void World::setMap(Map* map) {
	m_map = map;
}
