/*
 * Data simulator for mobile phone network events
 *
 * Constants.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: Bogdan Oancea
 */

#include <Constants.h>

const double Constants::POWER_THRESHOLD = 10;
const int Constants::NO_PERSONS = 4;
const int Constants::NO_ANTENNAS = 10;
const double Constants::MARKET_SHARE = 0.3;
const int Constants::NO_MOBILE_PHONES = Constants::NO_PERSONS * Constants::MARKET_SHARE;
