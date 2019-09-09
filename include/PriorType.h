/*
 * Data simulator for mobile phone network events
 *
 * MovementType.h
 *
 *  Created on: May 15, 2019
 *      Author: Bogdan Oancea
 */

#ifndef PRIORTYPE_H_
#define PRIORTYPE_H_

/**
 * An enum class that enumerates the types of the prior used to computed the posterior localization probability.
 * UNIFORM : the prior is an uniform probability, i.e. each object is equally located in each tile of the map.
 * REGISTER: the prior probability is given by an administrative register.
 * NETWORK: the prior probability is given by the mobile network - it is computed as the ratio between the signal quality
 * given by Antenna a in tile t and the sum of the signal quality given by all antennas in all tiles of the grid.
 */
enum class PriorType {
	UNIFORM, REGISTER, NETWORK
};


#endif /* MOVEMENTTYPE_H_ */
