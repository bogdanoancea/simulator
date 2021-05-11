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
#include <parsers/SimConfigParser.h>
#include <Utils.h>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <memory>
#include <sstream>

using namespace utils;
using namespace std;

SimConfigParser::SimConfigParser(const string& filename, AgentsCollection* agents, Map* map) :
		ConfigParser(filename) {

	m_simConfig = new SimulationConfiguration();
	m_simConfig->setMap(map);
	m_simConfig->setHomeWorkScenario(nullptr);
	m_agents = agents;
//	for (unsigned long i = 0; i < m_simConfig->getMnos().size(); i++)
//		agents->addAgent(m_simConfig->getMnos()[i]);
}

SimConfigParser::~SimConfigParser() {
}

void SimConfigParser::parse() {
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

shared_ptr<Distribution> SimConfigParser::parseStayTimeDistribution(XMLElement* parent) {
	shared_ptr<Distribution> result;
	XMLElement *ts = parent->FirstChildElement("time_stay");
	if (ts) {
		const XMLAttribute *type = ts->FindAttribute("distributionType");
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


shared_ptr<Distribution> SimConfigParser::parseIntervalBetweenStaysDistribution(XMLElement* parent) {
	shared_ptr<Distribution> result;
	XMLElement *its = parent->FirstChildElement("interval_between_stays");
	if (its) {
		const XMLAttribute *type = its->FindAttribute("distributionType");
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


void SimConfigParser::parseHomeWorkScenario(XMLElement* homeWorkElement, HomeWorkScenario* hws) {
	XMLElement* home = homeWorkElement->FirstChildElement("home");
	if(!home) {
		throw std::runtime_error("Home location missing!");
	}

	double x = getValue(home, "x_home");
	double y = getValue(home, "y_home");
	double xSd = getValue(home, "x_sd_home");
	double ySd = getValue(home, "y_sd_home");
	HomeWorkLocation h(x,y,0,xSd,ySd,0);
	hws->addHomeLocation(h);
	while( (home = home->NextSiblingElement("home")) != nullptr ) {
		x = getValue(home, "x_home");
		y = getValue(home, "y_home");
		xSd = getValue(home, "x_sd_home");
		ySd = getValue(home, "y_sd_home");
		HomeWorkLocation h1(x,y,0,xSd,ySd,0);
		hws->addHomeLocation(h1);
	}

	XMLElement* work = homeWorkElement->FirstChildElement("work");
	if(!work) {
		throw std::runtime_error("Work location missing!");
	}
	x = getValue(work, "x_work");
	y = getValue(work, "y_work");
	xSd = getValue(work, "x_sd_work");
	ySd = getValue(work, "y_sd_work");
	HomeWorkLocation w(x,y,0,xSd,ySd,0);
	hws->addWorkLocation(w);
	while( (work = work->NextSiblingElement("work")) != nullptr ) {
		x = getValue(work, "x_work");
		y = getValue(work, "y_work");
		xSd = getValue(work, "x_sd_work");
		ySd = getValue(work, "y_sd_work");
		HomeWorkLocation w1(x,y,0,xSd,ySd,0);
		hws->addWorkLocation(w1);
	}
	XMLElement* anchor = homeWorkElement->FirstChildElement("anchor_point");
	if (anchor) {
		x = getValue(anchor, "x_anchor");
		y = getValue(anchor, "y_anchor");
		xSd = getValue(anchor, "x_sd_anchor");
		ySd = getValue(anchor, "y_sd_anchor");
		HomeWorkLocation a(x, y, 0, xSd, ySd, 0);
		hws->addAnchorLocation(a);
		while ((anchor = anchor->NextSiblingElement("anchor_point")) != nullptr) {
			x = getValue(anchor, "x_anchor");
			y = getValue(anchor, "y_anchor");
			xSd = getValue(anchor, "x_sd_anchor");
			ySd = getValue(anchor, "y_sd_anchor");
			HomeWorkLocation a1(x, y, 0, xSd, ySd, 0);
			hws->addAnchorLocation(a1);
		}
	}
	hws->setPrecentTimeHome(getValue(homeWorkElement, "percent_time_home"));
	hws->setPrecentTimeWork(getValue(homeWorkElement, "percent_time_work"));
	hws->setPrecentTimeAnchorPoint(getValue(homeWorkElement, "percent_time_anchor_point"));
	hws->setPrecentTimeTravel(getValue(homeWorkElement, "percent_time_travel"));
	hws->setProbAnchorPoint(getValue(homeWorkElement, "prob_anchor_point"));
	Distribution* p = parseDirectionAngleDistribution(homeWorkElement);
	hws->setAngleDistribution(p);
}

Distribution* SimConfigParser::parseDirectionAngleDistribution(XMLElement* homeWorkElement) {
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



vector<MobileOperator*> SimConfigParser::parseMNOs(XMLElement* el) {
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

MovementType SimConfigParser::parseMovement(XMLElement* el) {
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
			} else
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

void SimConfigParser::parseLevyFlight(XMLElement* mvEl, LevyFlightScenario* lfs) {
	lfs->setSpeedDistribution(parseSpeedDistribution(mvEl));
	lfs->setCutOffPoint(getValue(mvEl, "cutOffSpeed", Constants::CUTOFFSPEED));
}

void SimConfigParser::parseRandomWalkDrift(XMLElement* mvEl, RandomWalkDriftScenario* rws) {
	rws->setTrendAngle1Distribution(parseTrendAngleDistribution(mvEl,1));
	rws->setTrendAngle2Distribution(parseTrendAngleDistribution(mvEl,2));
	Distribution* retAngle = parseReturnAngleDistribution(mvEl);
	rws->setReturnAngleDistribution(retAngle);
}

Distribution* SimConfigParser::parseSpeedDistribution(XMLElement* mvEl) {
	Distribution *result = nullptr;
	XMLElement *distribution = mvEl->FirstChildElement("speed_distribution");
	DistributionType dType;
	if (distribution) {
		const XMLAttribute *type = distribution->FindAttribute("distributionType");
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


Distribution* SimConfigParser::parseTrendAngleDistribution(XMLElement* mvEl, int noAngle) {
	Distribution *result = nullptr;
	ostringstream elem;
	elem << "trend_angle_" << noAngle << "_distribution";
	XMLElement *distribution = mvEl->FirstChildElement(elem.str().c_str());
	DistributionType dType;
	if (distribution) {
		const XMLAttribute *type = distribution->FindAttribute("distributionType");
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

Distribution* SimConfigParser::parseReturnAngleDistribution(XMLElement* mvEl) {
	Distribution *result = nullptr;
	XMLElement *distribution = mvEl->FirstChildElement("return_angle_distribution");
	DistributionType dType;
	if (distribution) {
		const XMLAttribute *type = distribution->FindAttribute("distributionType");
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


HoldableAgent::CONNECTION_TYPE SimConfigParser::parseConnectionType(XMLElement* el) {
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

double SimConfigParser::getDefaultConnectionThreshold(HoldableAgent::CONNECTION_TYPE connType) {
	double result = -1;
	if (connType == HoldableAgent::CONNECTION_TYPE::USING_POWER)
		result = Constants::PHONE_POWER_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
		result = Constants::PHONE_QUALITY_THRESHOLD;
	else if (connType == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
		result = Constants::PHONE_STRENGTH_THRESHOLD;
	return (result);
}

SimulationConfiguration* SimConfigParser::getSimulationConfiguration() {
	return m_simConfig;
}
