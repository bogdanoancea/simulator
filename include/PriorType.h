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

#ifndef PRIORTYPE_H_
#define PRIORTYPE_H_

/**
 * An enum class that enumerates the types of the prior used to computed the posterior localization probability.
 * \li UNIFORM : the prior is an uniform probability, i.e. each object is equally located in each tile of the map.
 * \li REGISTER: the prior probability is given by an administrative register.
 * \li NETWORK: the prior probability is given by the mobile network - it is computed as the ratio between the signal quality
 * given by Antenna a in tile t and the sum of the signal quality given by all antennas in all tiles of the grid.
 */
enum class PriorType {
	UNIFORM, REGISTER, NETWORK
};


#endif /* MOVEMENTTYPE_H_ */
