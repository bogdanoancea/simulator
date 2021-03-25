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
 * SimConfig.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: Bogdan.Oancea
 */

#include <Clock.h>
#include <Constants.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Coordinate.inl>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <IDGenerator.h>
#include <map/Map.h>
#include <parsers/HomeWorkLocation.h>
#include <parsers/SimConfig.h>
#include <Utils.h>
#include <cstring>
#include <filesystem>
#include <iostream>

using namespace utils;

SimConfig::SimConfig(const string& filename, AgentsCollection* agents, Map* map) :
		Config(filename) {

	m_map = map;
	m_homeWorkScenario = nullptr;
	parse();
	m_clock = new Clock(getStartTime(), getEndTime(), getTimeIncrement());
	for (unsigned long i = 0; i < m_mnos.size(); i++)
		agents->addAgent(m_mnos[i]);
}

SimConfig::~SimConfig() {
	if(m_homeWorkScenario)
		delete m_homeWorkScenario;
	delete m_clock;
}

void SimConfig::parse() {
	XMLDocument doc;
	XMLError err = doc.LoadFile(getFileName().c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for simulation ");

	XMLElement* simEl = doc.FirstChildElement("simulation");
	if (!simEl)
		throw std::runtime_error("Syntax error in the configuration file for simulation ");
	else {
		m_outputDir = getValue(simEl, "output_dir", Constants::OUTPUT_DIR);
#ifdef OSX
		try {
			std::__fs::filesystem::create_directory(std::__fs::filesystem::path(m_outputDir.c_str()));
		} catch(std::__fs::filesystem::filesystem_error& e) {
			cout << "Cannot create output folder :" << m_outputDir;
			throw e;
		}
#else
		try {
			std::filesystem::create_directory(std::filesystem::path(m_outputDir.c_str()));
		} catch (std::filesystem::filesystem_error& e) {
			cout << "Cannot create output folder :" << m_outputDir;
			throw e;
		}
#endif
		m_startTime = getValue(simEl, "start_time", Constants::SIM_START_TIME);
		m_endTime = getValue(simEl, "end_time", Constants::SIM_END_TIME);
		m_timeIncrement = getValue(simEl, "time_increment", Constants::SIM_INCREMENT_TIME);
		m_stay = getValue(simEl, "time_stay", Constants::SIM_STAY_TIME);
		m_intevalBetweenStays = getValue(simEl, "interval_between_stays", Constants::SIM_INTERVAL_BETWEEN_STAYS);
		m_mnos = parseMNOs(simEl);
		m_probSecMobilePhone = getValue(simEl, "prob_sec_mobile_phone", Constants::SIM_PROB_SECOND_MOBILE_PHONE);
		m_mvType = parseMovement(simEl);
		m_connType = parseConnectionType(simEl);
		m_connThreshold = getValue(simEl, "conn_threshold", getDefaultConnectionThreshold(m_connType));
		m_gridFilename = getValue(simEl, "grid_file", Constants::GRID_FILE_NAME);
		m_personsFilename = getValue(simEl, "persons_file", Constants::PERSONS_FILE_NAME);
		m_antennasFilename = getValue(simEl, "antennas_file", Constants::ANTENNAS_FILE_NAME);
		m_GridDimTileX = getValue(simEl, "grid_dim_tile_x", Constants::GRID_DIM_TILE_X);
		m_GridDimTileY = getValue(simEl, "grid_dim_tile_y", Constants::GRID_DIM_TILE_Y);
		m_seed = getValue(simEl, "random_seed", Constants::RANDOM_SEED);
		m_eventType = getValue(simEl, "event_type", Constants::EVENTTYPE);
		XMLElement* homeWorkEl = simEl->FirstChildElement("specs_home_work");
		if(homeWorkEl) {
			m_homeWorkScenario = new HomeWorkScenario();
			parseHomeWorkScenario(homeWorkEl, m_homeWorkScenario);
			cout << m_homeWorkScenario->toString() << endl;
		}
	}
}


void SimConfig::parseHomeWorkScenario(XMLElement* homeWorkElement, HomeWorkScenario* hws) {
	XMLElement* home = homeWorkElement->FirstChildElement("home");
	if(!home) {
		throw std::runtime_error("Home  location missing!");
	}

	double x = getValue(home, "x_home");
	double y = getValue(home, "y_home");
	double xSd = getValue(home, "x_sd_home");
	double ySd = getValue(home, "y_sd_home");
	HomeWorkLocation h(x,y,0,xSd,ySd,0);
	hws->addHomeLocation(h);
	while( (home = home->NextSiblingElement("home")) != nullptr ) {
		double x = getValue(home, "x_home");
		double y = getValue(home, "y_home");
		double xSd = getValue(home, "x_sd_home");
		double ySd = getValue(home, "y_sd_home");
		HomeWorkLocation h(x,y,0,xSd,ySd,0);
		hws->addHomeLocation(h);
	}

	XMLElement* work = homeWorkElement->FirstChildElement("work");
	x = getValue(work, "x_work");
	y = getValue(work, "y_work");
	xSd = getValue(work, "x_sd_work");
	ySd = getValue(work, "y_sd_work");
	HomeWorkLocation w(x,y,0,xSd,ySd,0);
	hws->addWorkLocation(w);
	while( (work = work->NextSiblingElement("work")) != nullptr ) {
		double x = getValue(work, "x_work");
		double y = getValue(work, "y_work");
		double xSd = getValue(work, "x_sd_work");
		double ySd = getValue(work, "y_sd_work");
		HomeWorkLocation w(x,y,0,xSd,ySd,0);
		hws->addWorkLocation(w);
	}
	hws->setPrecentTimeHome(getValue(homeWorkElement, "percent_time_home"));
	hws->setPrecentTimeWork(getValue(homeWorkElement, "percent_time_work"));
}

const string& SimConfig::getAntennasFilename() const {
	return m_antennasFilename;
}

void SimConfig::setAntennasFilename(const string& antennasFilename) {
	m_antennasFilename = antennasFilename;
}

double SimConfig::getConnThreshold() const {
	return m_connThreshold;
}

void SimConfig::setConnThreshold(double connThreshold) {
	m_connThreshold = connThreshold;
}

HoldableAgent::CONNECTION_TYPE SimConfig::getConnType() const {
	return m_connType;
}

void SimConfig::setConnType(HoldableAgent::CONNECTION_TYPE connType) {
	m_connType = connType;
}

unsigned long SimConfig::getEndTime() const {
	return m_endTime;
}

void SimConfig::setEndTime(unsigned long endTime) {
	m_endTime = endTime;
}

EventType SimConfig::getEventType() const {
	return m_eventType;
}

void SimConfig::setEventType(EventType eventType) {
	m_eventType = eventType;
}

double SimConfig::getGridDimTileX() const {
	return m_GridDimTileX;
}

void SimConfig::setGridDimTileX(double gridDimTileX) {
	m_GridDimTileX = gridDimTileX;
}

double SimConfig::getGridDimTileY() const {
	return m_GridDimTileY;
}

void SimConfig::setGridDimTileY(double gridDimTileY) {
	m_GridDimTileY = gridDimTileY;
}

const string& SimConfig::getGridFilename() const {
	return m_gridFilename;
}

void SimConfig::setGridFilename(const string& gridFilename) {
	m_gridFilename = gridFilename;
}

unsigned SimConfig::getIntevalBetweenStays() const {
	return m_intevalBetweenStays;
}

void SimConfig::setIntevalBetweenStays(unsigned intevalBetweenStays) {
	m_intevalBetweenStays = intevalBetweenStays;
}

const vector<MobileOperator*>& SimConfig::getMnos() const {
	return m_mnos;
}

void SimConfig::setMnos(const vector<MobileOperator*>& mnos) {
	m_mnos = mnos;
}

MovementType SimConfig::getMvType() const {
	return m_mvType;
}

void SimConfig::setMvType(MovementType mvType) {
	m_mvType = mvType;
}

const string& SimConfig::getOutputDir() const {
	return m_outputDir;
}

void SimConfig::setOutputDir(const string& outputDir) {
	m_outputDir = outputDir;
}

const string& SimConfig::getPersonsFilename() const {
	return m_personsFilename;
}

void SimConfig::setPersonsFilename(const string& personsFilename) {
	m_personsFilename = personsFilename;
}

double SimConfig::getProbSecMobilePhone() const {
	return m_probSecMobilePhone;
}

void SimConfig::setProbSecMobilePhone(double probSecMobilePhone) {
	m_probSecMobilePhone = probSecMobilePhone;
}

unsigned SimConfig::getSeed() const {
	return m_seed;
}

void SimConfig::setSeed(unsigned seed) {
	m_seed = seed;
}

unsigned long SimConfig::getStartTime() const {
	return m_startTime;
}

void SimConfig::setStartTime(unsigned long startTime) {
	m_startTime = startTime;
}

unsigned long SimConfig::getStay() const {
	return m_stay;
}

void SimConfig::setStay(unsigned long stay) {
	m_stay = stay;
}

unsigned long SimConfig::getTimeIncrement() const {
	return m_timeIncrement;
}

void SimConfig::setTimeIncrement(unsigned long timeIncrement) {
	m_timeIncrement = timeIncrement;
}

Clock* SimConfig::getClock() {
	return m_clock;
}

Map* SimConfig::getMap() {
	return m_map;
}


vector<MobileOperator*> SimConfig::parseMNOs(XMLElement* el) {
	vector<MobileOperator*> result;
	unsigned numMNO = 0;
	XMLElement* mnoEl = utils::getFirstChildElement(el, "mno");
	if (mnoEl) {
		for (; mnoEl; mnoEl = mnoEl->NextSiblingElement("mno")) {
			numMNO++;
			const char* name = getValue(mnoEl, "mno_name", "UNKNOWN");
			const double prob = getValue(mnoEl, "prob_mobile_phone");
			unsigned long id = IDGenerator::instance()->next();
			MobileOperator* mo = new MobileOperator(m_map, id, m_clock, name, prob, m_outputDir);
			result.push_back(mo);
		}
	} else {
		throw std::runtime_error("No MNO defined! At least one MNO should be defined for a valid simulation");
	}
	if (numMNO > 2)
		throw std::runtime_error("Maximum 2 MNOs are supported!");
	return (result);
}

MovementType SimConfig::parseMovement(XMLElement* el) {
	MovementType result;
	const char* mvType = getValue(el, "movement_type", "UNKNOWN");
	if (!strcmp(mvType, "random_walk_closed_map"))
		result = MovementType::RANDOM_WALK_CLOSED_MAP;
	else if (!strcmp(mvType, "random_walk_closed_map_drift")) {
		result = MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT;
	} else if (!strcmp(mvType, "levy_flight")) {
		result = MovementType::LEVY_FLIGHT;
	} else if(!strcmp(mvType, "home_work")) {
		result = MovementType::HOME_WORK;
	} else
		throw runtime_error("Unknown displacement mechanism!");
	return (result);
}

HoldableAgent::CONNECTION_TYPE SimConfig::parseConnectionType(XMLElement* el) {
	HoldableAgent::CONNECTION_TYPE result;
	const char* connType = getValue(el, "connection_type", "UNKNOWN");
	if (!strcmp(connType, "power"))
		result = HoldableAgent::CONNECTION_TYPE::USING_POWER;
	else if (!strcmp(connType, "quality"))
		result = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY;
	else if (!strcmp(connType, "strength"))
		result = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH;
	else
		throw runtime_error("Unknown connection mechanism! Available values: power, quality, strength");
	return (result);
}

double SimConfig::getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType) {
	double result = -1;
	if (connType == HoldableAgent::CONNECTION_TYPE::USING_POWER)
		result = Constants::PHONE_POWER_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
		result = Constants::PHONE_QUALITY_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
		result = Constants::PHONE_STRENGTH_THRESHOLD;
	return (result);
}

bool SimConfig::isHomeWorkScenario() const {
	return (m_homeWorkScenario != nullptr);
}


unsigned int SimConfig::getNumHomeLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getHomeLocations().size();
	else return -1;
}

unsigned int SimConfig::getNumWorkLocations() const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getWorkLocations().size();
	else return -1;
}

HomeWorkLocation SimConfig::getHomeLocation(unsigned int i) const {
	if(isHomeWorkScenario())
		return m_homeWorkScenario->getHomeLocations().at(i);
	else {
		throw std::runtime_error("No Home - Work scenario defined!");
	}
}
HomeWorkScenario* SimConfig::getHomeWorkScenario() {
	if(isHomeWorkScenario())
		return m_homeWorkScenario;
	else {
		return nullptr;
	}

}
