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
 * SimulationConfiguration.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <parsers/SimulationConfiguration.h>

SimulationConfiguration::SimulationConfiguration() {
	m_manhattanScenario = nullptr;
	m_homeWorkScenario = nullptr;
	m_homeWorkManhattanScenario = nullptr;
	m_randomWalkDriftScenario = nullptr;
	m_levyFlightScenario = nullptr;

}

SimulationConfiguration::~SimulationConfiguration() {
	if(m_homeWorkScenario)
		delete m_homeWorkScenario;
	if(m_homeWorkManhattanScenario)
		delete m_homeWorkManhattanScenario;
	if(m_randomWalkDriftScenario)
		delete m_randomWalkDriftScenario;
	if(m_levyFlightScenario)
		delete m_levyFlightScenario;
	if(m_manhattanScenario)
		delete m_manhattanScenario;
	delete m_clock;

}

const string& SimulationConfiguration::getAntennasFilename() const {
	return m_antennasFilename;
}

void SimulationConfiguration::setAntennasFilename(const string& antennasFilename) {
	m_antennasFilename = antennasFilename;
}

double SimulationConfiguration::getConnThreshold() const {
	return m_connThreshold;
}

void SimulationConfiguration::setConnThreshold(double connThreshold) {
	m_connThreshold = connThreshold;
}

HoldableAgent::CONNECTION_TYPE SimulationConfiguration::getConnType() const {
	return m_connType;
}

void SimulationConfiguration::setConnType(HoldableAgent::CONNECTION_TYPE connType) {
	m_connType = connType;
}

unsigned long SimulationConfiguration::getEndTime() const {
	return m_endTime;
}

void SimulationConfiguration::setEndTime(unsigned long endTime) {
	m_endTime = endTime;
}

EventType SimulationConfiguration::getEventType() const {
	return m_eventType;
}

void SimulationConfiguration::setEventType(EventType eventType) {
	m_eventType = eventType;
}

double SimulationConfiguration::getGridDimTileX() const {
	return m_GridDimTileX;
}

void SimulationConfiguration::setGridDimTileX(double gridDimTileX) {
	m_GridDimTileX = gridDimTileX;
}

double SimulationConfiguration::getGridDimTileY() const {
	return m_GridDimTileY;
}

void SimulationConfiguration::setGridDimTileY(double gridDimTileY) {
	m_GridDimTileY = gridDimTileY;
}

const string& SimulationConfiguration::getGridFilename() const {
	return m_gridFilename;
}

void SimulationConfiguration::setGridFilename(const string& gridFilename) {
	m_gridFilename = gridFilename;
}

shared_ptr<Distribution> SimulationConfiguration::getIntevalBetweenStays() const {
	return m_intevalBetweenStays;
}

void SimulationConfiguration::setIntevalBetweenStays(shared_ptr<Distribution> intevalBetweenStays) {
	m_intevalBetweenStays = intevalBetweenStays;
}

const vector<MobileOperator*>& SimulationConfiguration::getMnos() const {
	return m_mnos;
}

void SimulationConfiguration::setMnos(const vector<MobileOperator*>& mnos) {
	m_mnos = mnos;
}

MovementType SimulationConfiguration::getMvType() const {
	return m_mvType;
}

void SimulationConfiguration::setMvType(MovementType mvType) {
	m_mvType = mvType;
}

const string& SimulationConfiguration::getOutputDir() const {
	return m_outputDir;
}

void SimulationConfiguration::setOutputDir(const string& outputDir) {
	m_outputDir = outputDir;
}

const string& SimulationConfiguration::getPersonsFilename() const {
	return m_personsFilename;
}

void SimulationConfiguration::setPersonsFilename(const string& personsFilename) {
	m_personsFilename = personsFilename;
}

double SimulationConfiguration::getProbSecMobilePhone() const {
	return m_probSecMobilePhone;
}

void SimulationConfiguration::setProbSecMobilePhone(double probSecMobilePhone) {
	m_probSecMobilePhone = probSecMobilePhone;
}

unsigned SimulationConfiguration::getSeed() const {
	return m_seed;
}

void SimulationConfiguration::setSeed(unsigned seed) {
	m_seed = seed;
}

unsigned long SimulationConfiguration::getStartTime() const {
	return m_startTime;
}

void SimulationConfiguration::setStartTime(unsigned long startTime) {
	m_startTime = startTime;
}

shared_ptr<Distribution> SimulationConfiguration::getStay() const {
	return m_stay;
}

void SimulationConfiguration::setStay(shared_ptr<Distribution> stay) {
	m_stay = stay;
}

unsigned long SimulationConfiguration::getTimeIncrement() const {
	return m_timeIncrement;
}

void SimulationConfiguration::setTimeIncrement(unsigned long timeIncrement) {
	m_timeIncrement = timeIncrement;
}

Clock* SimulationConfiguration::getClock() {
	return m_clock;
}

Map* SimulationConfiguration::getMap() {
	return m_map;
}
void SimulationConfiguration::setMap(Map* map) {
	m_map = map;
}

bool SimulationConfiguration::isHomeWorkScenario() const {
	return (m_homeWorkScenario != nullptr);
}

bool SimulationConfiguration::isHomeWorkManhattanScenario() const {
	return (m_homeWorkManhattanScenario != nullptr);
}

unsigned int SimulationConfiguration::getNumHomeLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getHomeLocations().size();
	else if(isHomeWorkManhattanScenario())
		return m_homeWorkManhattanScenario->getHomeLocations().size();
	else return -1;
}

unsigned int SimulationConfiguration::getNumWorkLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getWorkLocations().size();
	else if(isHomeWorkManhattanScenario())
		return m_homeWorkManhattanScenario->getWorkLocations().size();
	else return -1;
}

unsigned int SimulationConfiguration::getNumAnchorLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getAnchorLocations().size();
	else if(isHomeWorkManhattanScenario())
		return m_homeWorkManhattanScenario->getAnchorLocations().size();
	else return -1;
}

HomeWorkLocation SimulationConfiguration::getHomeLocation(unsigned int i) const {
	if(isHomeWorkScenario() )
		return m_homeWorkScenario->getHomeLocations().at(i);
	else if(isHomeWorkManhattanScenario())
		return m_homeWorkManhattanScenario->getHomeLocations().at(i);
	else {
		throw std::runtime_error("No Home - Work scenario defined!");
	}
}

HomeWorkScenario* SimulationConfiguration::getHomeWorkScenario() {
	if(isHomeWorkScenario())
		return m_homeWorkScenario;
	else if(isHomeWorkManhattanScenario()){
		return m_homeWorkManhattanScenario;
	}
	else return nullptr;

}

HomeWorkLocation SimulationConfiguration::getWorkLocation(unsigned int i) const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getWorkLocations().at(i);
	else if(isHomeWorkManhattanScenario())
		return m_homeWorkManhattanScenario->getWorkLocations().at(i);
	else {
		throw std::runtime_error("No Home - Work scenario defined!");
	}
}

void SimulationConfiguration::setHomeWorkScenario(HomeWorkScenario* hws) {
	m_homeWorkScenario = hws;
}

void SimulationConfiguration::setClock() {
	m_clock = new Clock(getStartTime(), getEndTime(), getTimeIncrement());
}

RandomWalkDriftScenario* SimulationConfiguration::getRandomWalkDriftScenario() const {
	return m_randomWalkDriftScenario;
}

void SimulationConfiguration::setRandomWalkDriftScenario(RandomWalkDriftScenario *randomWalkDriftScenario) {
	m_randomWalkDriftScenario = randomWalkDriftScenario;
}

LevyFlightScenario* SimulationConfiguration::getLevyFlightScenario() const {
	return m_levyFlightScenario;
}

void SimulationConfiguration::setLevyFlightScenario(LevyFlightScenario *levyFlightScenario) {
	m_levyFlightScenario = levyFlightScenario;
}

ManhattanScenario* SimulationConfiguration::getManhattanScenario() const {
	return m_manhattanScenario;
}

void SimulationConfiguration::setManhattanScenario(ManhattanScenario *ms) {
	m_manhattanScenario = ms;
}

HomeWorkManhattanScenario* SimulationConfiguration::getHomeWorkManhattanScenario() const {
	if(isHomeWorkManhattanScenario())
		return m_homeWorkManhattanScenario;
	else {
		return nullptr;
	}
}

void SimulationConfiguration::setHomeWorkManhattanScenario(HomeWorkManhattanScenario* hwms) {
	m_homeWorkManhattanScenario = hwms;
}
