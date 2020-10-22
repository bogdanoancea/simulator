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
 * Constants.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <Constants.h>
#include <Utils.h>


const double Constants::PHONE_POWER_THRESHOLD = 2;
const double Constants::PHONE_QUALITY_THRESHOLD = 0.3;
const double Constants::PHONE_STRENGTH_THRESHOLD = -80;
const double Constants::PHONE_CONNECTION_THRESHOLD = -80;

const double Constants::ATT_FACTOR = 2;


const double Constants::ANTENNA_S_MID = -92.5;
const double Constants::ANTENNA_S_STEEP = 0.2;
const unsigned long Constants::SIM_NO_PERSONS = 100;
const unsigned long Constants::SIM_NO_ANTENNAS = 10;
const double Constants::SIM_PROB_MOBILE_PHONE = 0.35;
const double Constants::SIM_PROB_SECOND_MOBILE_PHONE = 0.1;
const unsigned long Constants::SIM_NO_MOBILE_PHONES = Constants::SIM_NO_PERSONS * Constants::SIM_PROB_MOBILE_PHONE;

const unsigned long Constants::SIM_START_TIME = 0;
const unsigned long Constants::SIM_END_TIME = 200;
const unsigned long Constants::SIM_INCREMENT_TIME = 1;
const unsigned long Constants::SIM_STAY_TIME = 5;
const unsigned long Constants::SIM_INTERVAL_BETWEEN_STAYS = 25;

const char Constants::sep = ',';
const char* Constants::GRID_FILE_NAME = "grid.csv";
const char* Constants::PROB_FILE_NAME_PREFIX = "probabilities";
const char* Constants::PERSONS_FILE_NAME = "persons.csv";
const char* Constants::ANTENNAS_FILE_NAME = "antennas.csv";
const char* Constants::OUTPUT_DIR = "output";

//const unsigned long Constants::GRID_NO_TILES_X = 10;
//const unsigned long Constants::GRID_NO_TILES_Y = 10;
const double Constants::GRID_DIM_TILE_X = 1000;
const double Constants::GRID_DIM_TILE_Y = 1000;

const PriorType Constants::PRIOR_PROBABILITY = PriorType::UNIFORM;
const NetworkType Constants::NETWORK_TYPE = NetworkType::_3G;

const double Constants::ANTENNA_HEIGHT = 0.0;
const double Constants::ANTENNA_TILT = 0.0;
const double Constants::ANTENNA_AZIM_DB_BACK = -30;
const double Constants::ANTENNA_ELEV_DB_BACK = -30;
const double Constants::ANTENNA_BEAM_H = 65;
const double Constants::ANTENNA_BEAM_V = 9;
const double Constants::ANTENNA_DIRECTION = 0;
const unsigned int Constants::ANTENNA_MAPPING_N = 1000;
const unsigned int Constants::ANTENNA_MIN_3_DB = 720;
const unsigned long Constants::ANTENNA_MAX_CONNECTIONS = 100;
const double Constants::ANTENNA_POWER = 10;
const double Constants::ANTENNA_SMIN = -80;
const double Constants::ANTENNA_QMIN = 0.3;

const unsigned int Constants::SIM_NUM_MNO = 1;
const int Constants::RANDOM_SEED = 123;

const char* Constants::SIM_DEFAULT_MNO_NAME = "MNO";

const double Constants::SIM_TREND_ANGLE_1 = 3 * utils::PI / 4;
const double Constants::SIM_TREND_ANGLE_2 = 5 * utils::PI / 4;

const double Constants::ANNULUS_SIZE_3G = 554.0;
const double Constants::ANNULUS_SIZE_4G = 78.07;

const char* Constants::SOFTWARE_VERSION = "0.2.0";
const EventType Constants::EVENTTYPE = EventType::CELLID;
