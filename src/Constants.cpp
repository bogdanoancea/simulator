/*
 * Data simulator for mobile phone network events
 *
 * Constants.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: Bogdan Oancea
 */

#include <Constants.h>


const double Constants::POWER_THRESHOLD = 2;
const double Constants::QUALITY_THRESHOLD = 0.5;
const double Constants::ATT_FACTOR = 2;
const int Constants::MAX_CONNECTIONS = 100;
const double Constants::ANTENNA_POWER = 10;
const double Constants::S_MID = -92.5;
const double Constants::S_STEEP = 0.2;
const unsigned long Constants::NO_PERSONS = 100;
const unsigned long Constants::NO_ANTENNAS = 10;
const double Constants::MARKET_SHARE = 0.3;
const unsigned long Constants::NO_MOBILE_PHONES = Constants::NO_PERSONS * Constants::MARKET_SHARE;

const unsigned long Constants::START_TIME = 0;
const unsigned long Constants::END_TIME = 200;
const unsigned long Constants::INCREMENT_TIME = 1;

const char Constants::sep = ',';
const char* Constants::GRID_FILE_NAME = "grid.csv";
const char* Constants::PROB_FILE_NAME = "prob.csv";
const char* Constants::PERSONS_FILE_NAME = "persons.csv";
const char* Constants::ANTENNAS_FILE_NAME = "antennas.csv";

//const unsigned long Constants::GRID_NO_TILES_X = 10;
//const unsigned long Constants::GRID_NO_TILES_Y = 10;
const double Constants::GRID_DIM_TILE_X = 1000;
const double Constants::GRID_DIM_TILE_Y = 1000;

const PriorType Constants::PRIOR_PROBABILITY = PriorType::UNIFORM;

const double Constants::ANTENNA_HEIGHT = 1000.0;
const double Constants::ANTENNA_TILT = 0.0;
const double Constants::ANTENNA_AZIM_DB_BACK = -30;
const double Constants::ANTENNA_ELEV_DB_BACK = -30;
const double Constants::ANTENNA_BEAM_H = 65;
const double Constants::ANTENNA_BEAM_V = 9;
const double Constants::ANTENNA_DIRECTION = 0;
const unsigned int Constants::ANTENNA_MAPPING_N = 1000;
const unsigned int Constants::ANTENNA_MIN_3_DB = 720;

const unsigned int Constants::NUM_MNO = 1;

