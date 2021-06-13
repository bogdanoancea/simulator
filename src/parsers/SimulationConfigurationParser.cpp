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
 * SimConfig.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/HoldableAgent.h>
#include <agent/MobileOperator.h>
#include <Clock.h>
#include <Constants.h>
#include <Distribution.h>
#include <events/EventType.h>
#include <IDGenerator.h>
#include <map/Map.h>
#include <parsers/HomeWorkLocation.h>
#include <parsers/SimulationConfigurationParser.h>
#include <Utils.h>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

using namespace utils;
using namespace std;

SimulationConfigurationParser::SimulationConfigurationParser(const string& filename, AgentsCollection* agents, Map* map) :
		ConfigParser(filename) {

	m_simConfig = new SimulationConfiguration();
	m_simConfig->setMap(map);
	m_agents = agents;
//	for (unsigned long i = 0; i < m_simConfig->getMnos().size(); i++)
//		agents->addAgent(m_simConfig->getMnos()[i]);
}

SimulationConfigurationParser::~SimulationConfigurationParser() {
}

void SimulationConfigurationParser::parse() {
	XMLDocument doc;
	XMLError err = doc.LoadFile(getFileName().c_str());
	if (err != XML_SUCCESS)
		throw std::runtime_error("Error opening configuration file for simulation ");

	XMLElement* simEl = doc.FirstChildElement("simulation");
	if (!simEl)
		throw std::runtime_error("Syntax error in the configuration file for simulation ");
	else {
		m_simConfig->setOutputDir(getValue(simEl, "output_dir", Constants::OUTPUT_DIR));
#ifdef OSX
		try {
			std::__fs::filesystem::create_directory(std::__fs::filesystem::path(m_simConfig->getOutputDir().c_str()));
		} catch(std::__fs::filesystem::filesystem_error& e) {
			cout << "Cannot create output folder :" << m_simConfig->getOutputDir();
			throw e;
		}
#else
		try {
			std::filesystem::create_directory(std::filesystem::path(m_simConfig->getOutputDir().c_str()));
		} catch (std::filesystem::filesystem_error& e) {
			cout << "Cannot create output folder :" << m_simConfig->getOutputDir();
			throw e;
		}
#endif
		m_simConfig->setStartTime(getValue(simEl, "start_time", Constants::SIM_START_TIME));
		m_simConfig->setEndTime(getValue(simEl, "end_time", Constants::SIM_END_TIME));
		m_simConfig->setTimeIncrement(getValue(simEl, "time_increment", Constants::SIM_INCREMENT_TIME));
		m_simConfig->setStay(parseStayTimeDistribution(simEl));
		m_simConfig->setIntevalBetweenStays(parseIntervalBetweenStaysDistribution(simEl));
		m_simConfig->setMnos(parseMNOs(simEl));
		m_simConfig->setProbSecMobilePhone(getValue(simEl, "prob_sec_mobile_phone", Constants::SIM_PROB_SECOND_MOBILE_PHONE));
		m_simConfig->setMvType(parseMovement(simEl));
		m_simConfig->setConnType(parseConnectionType(simEl));
		m_simConfig->setConnThreshold(getValue(simEl, "conn_threshold", getDefaultConnectionThreshold(m_simConfig->getConnType())));
		m_simConfig->setGridFilename(getValue(simEl, "grid_file", Constants::GRID_FILE_NAME));
		m_simConfig->setPersonsFilename(getValue(simEl, "persons_file", Constants::PERSONS_FILE_NAME));
		m_simConfig->setAntennasFilename(getValue(simEl, "antennas_file", Constants::ANTENNAS_FILE_NAME));
		m_simConfig->setGridDimTileX(getValue(simEl, "grid_dim_tile_x", Constants::GRID_DIM_TILE_X));
		m_simConfig->setGridDimTileY(getValue(simEl, "grid_dim_tile_y", Constants::GRID_DIM_TILE_Y));
		m_simConfig->setSeed(getValue(simEl, "random_seed", Constants::RANDOM_SEED));
		m_simConfig->setEventType(getValue(simEl, "event_type", Constants::EVENTTYPE));
		m_simConfig->setClock();
		for (unsigned long i = 0; i < m_simConfig->getMnos().size(); i++)
			m_agents->addAgent(m_simConfig->getMnos()[i]);
	}
}

shared_ptr<Distribution> SimulationConfigurationParser::parseStayTimeDistribution(XMLElement* parent) {
	shared_ptr<Distribution> result;
	XMLElement *ts = parent->FirstChildElement("time_stay_distribution");
	if (ts) {
		const XMLAttribute *type = ts->FindAttribute("type");
		DistributionType dType;
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Uniform")) {
				dType = DistributionType::UNIFORM;
			} else if (!strcmp(dname, "Normal")) {
				dType = DistributionType::NORMAL;
			} else {
				throw std::runtime_error("Unknown/unsupported time stay distribution");
			}
		} else {
			throw std::runtime_error("Time stay distribution type not specified ");
		}
		result = make_shared<Distribution>(Distribution(dType, ts));
	} else {
		throw std::runtime_error("Time stay distribution not specified ");
	}
	return result;

}


shared_ptr<Distribution> SimulationConfigurationParser::parseIntervalBetweenStaysDistribution(XMLElement* parent) {
	shared_ptr<Distribution> result;
	XMLElement *its = parent->FirstChildElement("interval_between_stays_distribution");
	if (its) {
		const XMLAttribute *type = its->FindAttribute("type");
		DistributionType dType;
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Exponential")) {
				dType = DistributionType::EXPONENTIAL;
			}
			else {
				throw std::runtime_error("Unknown/unsupported interval between stays distribution");
			}
		} else {
			throw std::runtime_error("Interval between stays distribution type not specified ");
		}
		result = make_shared<Distribution>(Distribution(dType, its));
	} else {
		throw std::runtime_error("Interval between stays distribution not specified ");
	}
	return result;
}

void SimulationConfigurationParser::addLocation(const char* name, HomeWorkLocation h, HomeWorkScenario* hws) {
	if(!strcmp(name, "home"))
		hws->addHomeLocation(h);
	else if(!strcmp(name, "work"))
		hws->addWorkLocation(h);
	else if(!strcmp(name, "anchor_point"))
		hws->addAnchorLocation(h);
}

void SimulationConfigurationParser::parseLocations(XMLElement* elem, const char* name, HomeWorkScenario* hws) {
	XMLElement* home = elem->FirstChildElement(name);
	if(!home) {
		ostringstream res;
		res << name << "  location missing!";
		throw std::runtime_error(res.str());
	}
	double x = getValue(home, "x");
	double y = getValue(home, "y");
	double xSd = getValue(home, "x_sd");
	double ySd = getValue(home, "y_sd");
	HomeWorkLocation h(x, y, 0, xSd, ySd, 0);
	addLocation(name, h, hws);
	while( (home = home->NextSiblingElement(name)) != nullptr ) {
		x = getValue(home, "x");
		y = getValue(home, "y");
		xSd = getValue(home, "x_sd");
		ySd = getValue(home, "y_sd");
		HomeWorkLocation h1(x, y, 0, xSd, ySd, 0);
		addLocation(name, h1, hws);
	}
}

void SimulationConfigurationParser::parseHomeWorkScenario(XMLElement* homeWorkElement, HomeWorkScenario* hws) {
	parseLocations(homeWorkElement, "home", hws);
	parseLocations(homeWorkElement, "work", hws);
	parseLocations(homeWorkElement, "anchor_point", hws);
	hws->setPrecentTimeHome(getValue(homeWorkElement, "percent_time_home"));
	hws->setPrecentTimeWork(getValue(homeWorkElement, "percent_time_work"));
	hws->setPrecentTimeAnchorPoint(getValue(homeWorkElement, "percent_time_anchor_point"));
	hws->setPrecentTimeTravel(getValue(homeWorkElement, "percent_time_travel"));
	hws->setProbAnchorPoint(getValue(homeWorkElement, "prob_anchor_point"));
	Distribution* p = parseDirectionAngleDistribution(homeWorkElement);
	hws->setAngleDistribution(p);
}


void SimulationConfigurationParser::parseHomeWorkManhattanScenario(XMLElement* homeWorkElement, HomeWorkScenario* hws) {
	parseLocations(homeWorkElement, "home", hws);
	parseLocations(homeWorkElement, "work", hws);
	parseLocations(homeWorkElement, "anchor_point", hws);
	XMLElement* manhattan_grid = homeWorkElement->FirstChildElement("manhattan_grid");
	if(manhattan_grid)
		parseManhattan(manhattan_grid, ((HomeWorkManhattanScenario*)hws)->getManhattanScenario());
	hws->setPrecentTimeHome(getValue(homeWorkElement, "percent_time_home"));
	hws->setPrecentTimeWork(getValue(homeWorkElement, "percent_time_work"));
	hws->setPrecentTimeAnchorPoint(getValue(homeWorkElement, "percent_time_anchor_point"));
	hws->setPrecentTimeTravel(getValue(homeWorkElement, "percent_time_travel"));
	hws->setProbAnchorPoint(getValue(homeWorkElement, "prob_anchor_point"));
}


Distribution* SimulationConfigurationParser::parseDirectionAngleDistribution(XMLElement* homeWorkElement) {
	Distribution* result = nullptr;
	XMLElement* distribution = homeWorkElement->FirstChildElement("direction_angle_distribution");
	DistributionType dType;
	if(distribution) {
		const XMLAttribute* type = distribution->FindAttribute("type");
		if(type) {
			const char* dname = type->Value();
			if(!strcmp(dname, "Laplace")) {
				dType = DistributionType::LAPLACE;
			}
			else if(!strcmp(dname, "Normal")) {
				dType = DistributionType::NORMAL;
			}
			else if(!strcmp(dname, "Uniform")) {
				dType = DistributionType::UNIFORM;
			}
			else {
				throw std::runtime_error("Direction angle distribution type not supported");
			}
			result = new Distribution(dType, distribution);
		}
		else {
			throw std::runtime_error("Direction angle distribution type not specified");
		}
	}
	else {
		throw std::runtime_error("Direction angle distribution not specified");
	}
	return result;
}



vector<MobileOperator*> SimulationConfigurationParser::parseMNOs(XMLElement* el) {
	vector<MobileOperator*> result;
	unsigned numMNO = 0;
	XMLElement* mnoEl = utils::getFirstChildElement(el, "mno");
	if (mnoEl) {
		for (; mnoEl; mnoEl = mnoEl->NextSiblingElement("mno")) {
			numMNO++;
			const char* name = getValue(mnoEl, "mno_name", "UNKNOWN");
			const double prob = getValue(mnoEl, "prob_mobile_phone");
			unsigned long id = IDGenerator::instance()->next();
			MobileOperator* mo = new MobileOperator(m_simConfig->getMap(), id, m_simConfig->getClock(), name, prob, m_simConfig->getOutputDir());
			result.push_back(mo);
		}
	} else {
		throw std::runtime_error("No MNO defined! At least one MNO should be defined for a valid simulation");
	}
	if (numMNO > 2)
		throw std::runtime_error("Maximum 2 MNOs are supported!");
	return (result);
}

MovementType SimulationConfigurationParser::parseMovement(XMLElement* el) {
	MovementType result = MovementType::UNKNOWN;
	XMLElement* mvEl = el->FirstChildElement("movement_pattern");
	if(mvEl) {
		const XMLAttribute* type = mvEl->FindAttribute("type");
		if(type) {
			const char* mvType = type->Value();
			if (!strcmp(mvType, "random_walk_closed_map"))
				result = MovementType::RANDOM_WALK_CLOSED_MAP;
			else if (!strcmp(mvType, "random_walk_closed_map_drift")) {
				m_simConfig->setRandomWalkDriftScenario(new RandomWalkDriftScenario());
				parseRandomWalkDrift(mvEl, m_simConfig->getRandomWalkDriftScenario());
				result = MovementType::RANDOM_WALK_CLOSED_MAP_WITH_DRIFT;
			} else if (!strcmp(mvType, "levy_flight")) {
				m_simConfig->setLevyFlightScenario(new LevyFlightScenario());
				parseLevyFlight(mvEl, m_simConfig->getLevyFlightScenario());
				result = MovementType::LEVY_FLIGHT;
			} else if(!strcmp(mvType, "home_work")) {
				m_simConfig->setHomeWorkScenario(new HomeWorkScenario());
				parseHomeWorkScenario(mvEl, m_simConfig->getHomeWorkScenario());
				cout << m_simConfig->getHomeWorkScenario()->toString() << endl;
				result = MovementType::HOME_WORK;
			} else if(!strcmp(mvType, "manhattan")){
				ManhattanScenario* ms  = new ManhattanScenario();
				parseManhattan(mvEl, ms);
				m_simConfig->setManhattanScenario(ms);
				cout << m_simConfig->getManhattanScenario()->toString() << endl;
				result = MovementType::MANHATTAN;
			} else if (!strcmp(mvType, "home_work_manhattan")) {
				ManhattanScenario* ms  = new ManhattanScenario();
				parseManhattan(mvEl, ms);
				m_simConfig->setHomeWorkManhattanScenario(new HomeWorkManhattanScenario(ms));
				parseHomeWorkManhattanScenario(mvEl, m_simConfig->getHomeWorkManhattanScenario());
				cout << m_simConfig->getHomeWorkManhattanScenario()->toString() << endl;
				result = MovementType::HOME_WORK_MANHATTAN;
			}
			else
				throw runtime_error("Unknown movement mechanism!");
		}
		else {
			throw runtime_error("Movement mechanism type not specified!");
		}
	}
	else {
		throw runtime_error("Movement mechanism not specified!");
	}
	return (result);
}

void SimulationConfigurationParser::parseLevyFlight(XMLElement* mvEl, LevyFlightScenario* lfs) {
	lfs->setSpeedDistribution(parseSpeedDistribution(mvEl));
	lfs->setCutOffPoint(getValue(mvEl, "cutOffSpeed", Constants::CUTOFFSPEED));
}

void SimulationConfigurationParser::parseRandomWalkDrift(XMLElement* mvEl, RandomWalkDriftScenario* rws) {
	rws->setTrendAngle1Distribution(parseTrendAngleDistribution(mvEl,1));
	rws->setTrendAngle2Distribution(parseTrendAngleDistribution(mvEl,2));
	Distribution* retAngle = parseReturnAngleDistribution(mvEl);
	rws->setReturnAngleDistribution(retAngle);
}

void SimulationConfigurationParser::parseManhattan(XMLElement* mvEl, ManhattanScenario* ms) {
	ms->setXStep(getValue(mvEl, "x_step", Constants::GRID_DIM_TILE_X));
	ms->setYStep(getValue(mvEl, "y_step", Constants::GRID_DIM_TILE_Y));
	ms->setXOrigin(getValue(mvEl, "x_origin", Constants::GRID_X_ORIG));
	ms->setYOrigin(getValue(mvEl, "y_origin", Constants::GRID_Y_ORIG));
}

Distribution* SimulationConfigurationParser::parseSpeedDistribution(XMLElement* mvEl) {
	Distribution *result = nullptr;
	XMLElement *distribution = mvEl->FirstChildElement("speed_distribution");
	DistributionType dType;
	if (distribution) {
		const XMLAttribute *type = distribution->FindAttribute("type");
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Levy")) {
				dType = DistributionType::LEVY;
			} else {
				throw std::runtime_error("Speed distribution type not supported");
			}
			result = new Distribution(dType, distribution);
		} else {
			throw std::runtime_error("Speed distribution type not specified");
		}
	} else {
		throw std::runtime_error("Speed distribution not specified");
	}
	return result;
}


Distribution* SimulationConfigurationParser::parseTrendAngleDistribution(XMLElement* mvEl, int noAngle) {
	Distribution *result = nullptr;
	ostringstream elem;
	elem << "trend_angle_" << noAngle << "_distribution";
	XMLElement *distribution = mvEl->FirstChildElement(elem.str().c_str());
	DistributionType dType;
	if (distribution) {
		const XMLAttribute *type = distribution->FindAttribute("type");
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Normal")) {
				dType = DistributionType::NORMAL;
			} else if (!strcmp(dname, "Uniform")) {
				dType = DistributionType::UNIFORM;
			} else {
				throw std::runtime_error("Trend angle distribution type not supported");
			}
			result = new Distribution(dType, distribution);
		} else {
			throw std::runtime_error("Trend angle distribution type not specified");
		}
	} else {
		throw std::runtime_error("Trend angle distribution not specified");
	}
	return result;
}

Distribution* SimulationConfigurationParser::parseReturnAngleDistribution(XMLElement* mvEl) {
	Distribution *result = nullptr;
	XMLElement *distribution = mvEl->FirstChildElement("return_angle_distribution");
	DistributionType dType;
	if (distribution) {
		const XMLAttribute *type = distribution->FindAttribute("type");
		if (type) {
			const char *dname = type->Value();
			if (!strcmp(dname, "Normal")) {
				dType = DistributionType::NORMAL;
			} else if (!strcmp(dname, "Uniform")) {
				dType = DistributionType::UNIFORM;
			} else {
				throw std::runtime_error("Return angle distribution type not supported");
			}
			result = new Distribution(dType, distribution);
		} else {
			throw std::runtime_error("Return angle distribution type not specified");
		}
	} else {
		throw std::runtime_error("Return angle distribution not specified");
	}
	return result;
}


HoldableAgent::CONNECTION_TYPE SimulationConfigurationParser::parseConnectionType(XMLElement* el) {
	HoldableAgent::CONNECTION_TYPE result;
	const char* connType = getValue(el, "connection_type", "UNKNOWN");
	if (!strcmp(connType, "power"))
		result = HoldableAgent::CONNECTION_TYPE::USING_POWER;
	else if (!strcmp(connType, "quality"))
		result = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_DOMINANCE;
	else if (!strcmp(connType, "strength"))
		result = HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH;
	else
		throw runtime_error("Unknown connection mechanism! Available values: power, quality, strength");
	return (result);
}

double SimulationConfigurationParser::getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType) {
	double result = -1;
	if (connType == HoldableAgent::CONNECTION_TYPE::USING_POWER)
		result = Constants::PHONE_POWER_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_DOMINANCE)
		result = Constants::PHONE_QUALITY_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
		result = Constants::PHONE_STRENGTH_THRESHOLD;
	return (result);
}

SimulationConfiguration* SimulationConfigurationParser::getSimulationConfiguration() {
	return m_simConfig;
}
