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
 * MovementType.h
 *
 *  Created on: May 15, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef MOVEMENTTYPE_H_
#define MOVEMENTTYPE_H_

/**
 * An enum class that enumerates the types of the methods used to move the people on the map:
 * \li RANDOM_WALK_CLOSED_MAP - the agent moves randomly inside the map boundary. The direction is generated
 * as a random value at each time step and the step length is computed multiplying the speed with the time interval.
 * \li RANDOM_WALK_CLOSED_MAP_WITH_DRIFT:  the agent moves in a preferential direction. See the documentation of the
 * RnadomWalkDriftScenario for more details.
 * \li LEVY_FLIGHT - the agent moves according to a "Levy flight" i.e. the travel distance for each time step
 * is distributed according to a Levy distribution.
 * \li HOME_WORK - the agent moves from a home location to a work location and optionally to an anchor point location. It stays
 * for a configurable time interval at each location.
 * \li MANHATTAN - the agent follows a Manhattan mobility patern.
 */
enum class MovementType {
	RANDOM_WALK_CLOSED_MAP, RANDOM_WALK_CLOSED_MAP_WITH_DRIFT, LEVY_FLIGHT, HOME_WORK, MANHATTAN, UNKNOWN
};


#endif /* MOVEMENTTYPE_H_ */
