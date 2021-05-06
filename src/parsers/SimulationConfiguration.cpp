/*
 * SimulationConfiguration.cpp
 *
 *  Created on: Apr 29, 2021
 *      Author: bogdan
 */

#include <parsers/SimulationConfiguration.h>

SimulationConfiguration::SimulationConfiguration() {

}

SimulationConfiguration::~SimulationConfiguration() {
	if(m_homeWorkScenario)
		delete m_homeWorkScenario;
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


unsigned int SimulationConfiguration::getNumHomeLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getHomeLocations().size();
	else return -1;
}

unsigned int SimulationConfiguration::getNumWorkLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getWorkLocations().size();
	else return -1;
}

unsigned int SimulationConfiguration::getNumAnchorLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getAnchorLocations().size();
	else return -1;
}

HomeWorkLocation SimulationConfiguration::getHomeLocation(unsigned int i) const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getHomeLocations().at(i);
	else {
		throw std::runtime_error("No Home - Work scenario defined!");
	}
}

HomeWorkScenario* SimulationConfiguration::getHomeWorkScenario() {
	if(isHomeWorkScenario())
		return m_homeWorkScenario;
	else {
		return nullptr;
	}

}

HomeWorkLocation SimulationConfiguration::getWorkLocation(unsigned int i) const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getWorkLocations().at(i);
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

