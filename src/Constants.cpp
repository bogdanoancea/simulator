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
const unsigned int Constants::SIMULATION_TIME = 220;
const char Constants::sep = ',';
const char* Constants::GRID_FILE_NAME = "grid.csv";
