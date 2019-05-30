/*
 * Data simulator for mobile phone network events
 *
 * MovementType.h
 *
 *  Created on: May 15, 2019
 *      Author: Bogdan Oancea
 */

#ifndef MOVEMENTTYPE_H_
#define MOVEMENTTYPE_H_

/**
 * An enum class that enumerates the types of the methods used to move the people on the map.
 */
enum class MovementType {
	RANDOM_WALK_CLOSED_MAP, RANDOM_WALK_WITH_DRIFT, UNKNOWN
};


#endif /* MOVEMENTTYPE_H_ */
