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
 * Constants.h
 *
 *  Created on: Apr 24, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <PriorType.h>
#include <events/EventType.h>
#include <NetworkType.h>
#include <string>

/**
 * These are some constants used in the process of the simulation, most of them are only used for testing and rapid development
 * of some methods, the real values of the parameters being read from the configuration files.
 */
class Constants {
public:

	/**
	 * If the signal received by a mobile device has a power below this level, the signal is considered only noise and unusable.
	 */
	static const double PHONE_POWER_THRESHOLD;

	/**
	 * If the signal received by a mobile device has a quality below this level, the signal is considered only noise and unusable.
	 */
	static const double PHONE_QUALITY_THRESHOLD;


	/**
	 * If the signal received by a mobile device has a quality below this level, the signal is considered only noise and unusable.
	 */
	static const double PHONE_STRENGTH_THRESHOLD;

	/**
	 * This value is interpreted according to the connection type:
	 *  - if the connection uses power it is the minimum value of the signal power received by a phone not considered as
	 *  noise. Below this value the signal is unusable and the connection between a mobile phone and an antenna is not possible.
	 *  - if the connection uses signal quality it is the minimum value of the signal quality received by a phone not considered as
	 *  noise. Below this value the signal is unusable and the connection between a mobile phone and an antenna is not possible.
	 *  - if the connection uses signal strength it is the minimum value of the signal strength received by a phone not considered as
	 *  noise. Below this value the signal is unusable and the connection between a mobile phone and an antenna is not possible.
	 */
	static const double PHONE_CONNECTION_THRESHOLD;

	/**
	 * Antenna power in Watts.
	 */
	static const double ANTENNA_POWER;

	/**
	 * Attenuation factor of the signal. It usually takes values between 2 in open field and 6 inside buildings
	 */
	static const double ATT_FACTOR;

	/**
	 * The maximum number of devices an antenna can connect.
	 */
	static const unsigned long ANTENNA_MAX_CONNECTIONS;

	/**
	 * The Smid parameter of an antenna
	 */
	static const double ANTENNA_S_MID;

	/**
	 * The Sstepp parameter of an antenna
	 */
	static const double ANTENNA_S_STEEP;

	/**
	 * The number of persons used for a simulation
	 */
	static const unsigned long SIM_NO_PERSONS;

	/**
	 * The number of antenna used for a simulation
	 */
	static const unsigned long SIM_NO_ANTENNAS;

	/**
	 * The number of the mobile devices used for a simulation
	 */
	static const unsigned long SIM_NO_MOBILE_PHONES;

	/**
	 * Default starting time of a simulation
	 */
	static const unsigned long SIM_START_TIME;

	/**
	 * Default ending time of a simulation
	 */
	static const unsigned long SIM_END_TIME;

	/**
	 * Default time increment for a simulation
	 */
	static const unsigned long SIM_INCREMENT_TIME;

	static const unsigned long SIM_STAY_TIME;

	static const unsigned long SIM_INTERVAL_BETWEEN_STAYS;

	/**
	 * The separator used when information is saved in output files
	 */
	static const char sep;

	/**
	 * The name of the file where the description of the grid is saved
	 */
	static const char* GRID_FILE_NAME;

	static const unsigned long GRID_X_ORIG;

	static const unsigned long GRID_Y_ORIG;

	static const double GRID_DIM_TILE_X;

	static const double GRID_DIM_TILE_Y;

	/**
	 * The name of the file where the probabilities of mobile phones locations are saved
	 */
	static const char* PROB_FILE_NAME_PREFIX;

	/**
	 * The name of the file where the exact positions of the persons are saved during simulation. They are needed for later analysis.
	 */
	static const char* PERSONS_FILE_NAME;

	/**
	 * The name of the file where the exact positions of the antennas are saved during simulation. They are needed for later analysis.
	 */
	static const char* ANTENNAS_FILE_NAME;

	/**
	 * The name of the folder where the output fle will be saved
	 */
	static const char* OUTPUT_DIR;

	/**
	 * Indicates how the prior probability is computed: uniform, register, network
	 */
	static const PriorType PRIOR_PROBABILITY;

	/**
	 * Indicates the type of network 3G or 4G
	 */
	static const NetworkType NETWORK_TYPE;


	/**
	 * the antenna height
	 */
	static const double ANTENNA_HEIGHT;


	static const double ANTENNA_TILT;


	static const double ANTENNA_AZIM_DB_BACK;

	static const double ANTENNA_ELEV_DB_BACK;

	static const double ANTENNA_BEAM_H;

	static const double ANTENNA_BEAM_V;

	static const double ANTENNA_DIRECTION;

	static const unsigned int ANTENNA_MAPPING_N;

	static const unsigned int ANTENNA_MIN_3_DB;

	static const double ANTENNA_SMIN;
	static const double ANTENNA_QMIN;

	static const unsigned int SIM_NUM_MNO;

	static const char* SIM_DEFAULT_MNO_NAME;

	static const double SIM_PROB_MOBILE_PHONE;
	static const double SIM_PROB_SECOND_MOBILE_PHONE;

	static const double SIM_TREND_ANGLE_1;
	static const double SIM_TREND_ANGLE_2;

	static const int RANDOM_SEED;

	static const double ANNULUS_SIZE_3G;
	static const double ANNULUS_SIZE_4G;

	static const char* SOFTWARE_VERSION;

	static const EventType EVENTTYPE;
	static const char* DEFAULT_ANGLE_DISTRIBUTION;
	static const double DEFAULT_SCALE_LAPLACE;
	static const double DEFAULT_MEAN_NORMAL;
	static const double DEFAULT_SD_NORMAL;
	static const double DEFAULT_MIN_UNIFORM;
	static const double DEFAULT_MAX_UNIFORM;

};

#endif /* CONSTANTS_H_ */
