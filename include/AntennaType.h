/*
 * Data simulator for mobile phone network events
 *
 * AntennaType.h
 *
 *  Created on: May 2, 2019
 *      Author: Bogdan Oancea
 */

#ifndef ANTENNATYPE_H_
#define ANTENNATYPE_H_

/**
 * An enum class that is used to represent the type of an antenna.
 * There are two types of antennas supported: omnidirectional and directional.
 */
enum class AntennaType {
	OMNIDIRECTIONAL, DIRECTIONAL
};

#endif /* ANTENNATYPE_H_ */
