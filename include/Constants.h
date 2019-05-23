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
	 * The total number of time steps for a simulation
	 */
	static const unsigned int SIMULATION_TIME;

	/**
	 * The separator used when information is saved in output files
	 */
	static const char sep;

	/**
	 * The name of the file where the description of the grid is saved
	 */
	static const char* GRID_FILE_NAME;
};

#endif /* CONSTANTS_H_ */
