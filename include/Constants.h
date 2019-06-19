/*
 * Data simulator for mobile phone network events
 *
 * Constants.h
 *
 *  Created on: Apr 24, 2019
 *      Author: Bogdan Oancea
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <PriorType.h>

/**
 * These are some constants used in the process of the simulation, most of them are only used for testing and rapid development
 * of some methods, the real values of the parameters being read from the configuration files.
 */
class Constants {
public:

	/**
	 * If the signal received by a mobile device has a power below this level, the signal is considered only noise and unusable.
	 */
	static const double POWER_THRESHOLD;

	/**
	 * If the signal received by a mobile device has a quality below this level, the signal is considered only noise and unusable.
	 */
	static const double QUALITY_THRESHOLD;

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
	static const int MAX_CONNECTIONS;

	/**
	 * The Smid parameter of an antenna
	 */
	static const double S_MID;

	/**
	 * The Sstepp parameter of an antenna
	 */
	static const double S_STEEP;

	/**
	 * The number of persons used for a simulation
	 */
	static const unsigned long NO_PERSONS;

	/**
	 * The number of antenna used for a simulation
	 */
	static const unsigned long NO_ANTENNAS;

	/**
	 * The market share of mobile phone operator, used to compute how many people have mobile devices
	 */
	static const double MARKET_SHARE;

	/**
	 * The number of the mobile devices used for a simulation
	 */
	static const unsigned long NO_MOBILE_PHONES;

	/**
	 * Default starting time of a simulation
	 */
	static const unsigned long START_TIME;

	/**
	 * Default ending time of a simulation
	 */
	static const unsigned long END_TIME;

	/**
	 * Default time increment for a simulation
	 */
	static const unsigned long INCREMENT_TIME;


	/**
	 * The separator used when information is saved in output files
	 */
	static const char sep;

	/**
	 * The name of the file where the description of the grid is saved
	 */
	static const char* GRID_FILE_NAME;

	/**
	 * Default number of tiles on OX in the grid
	 */
	//static const unsigned long GRID_NO_TILES_X;

	/**
	 * Default number of tiles on OY in the grid
	 */
	//static const unsigned long GRID_NO_TILES_Y;

	static const unsigned long GRID_X_ORIG;

	static const unsigned long GRID_Y_ORIG;

	static const double GRID_DIM_TILE_X;

	static const double GRID_DIM_TILE_Y;
	/**
	 * The name of the file where the probabilities of mobile phones locations are saved
	 */
	static const char* PROB_FILE_NAME;

	/**
	 * The name of the file where the exact positions of the persons are saved during simulation. They are needed for later analysis.
	 */
	static const char* PERSONS_FILE_NAME;

	/**
	 * The name of the file where the exact positions of the antennas are saved during simulation. They are needed for later analysis.
	 */
	static const char* ANTENNAS_FILE_NAME;

	/**
	 * Indicates how the prior probability is computed: uniform, register, network
	 */
	static const PriorType PRIOR_PROBABILITY;

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

};

#endif /* CONSTANTS_H_ */
