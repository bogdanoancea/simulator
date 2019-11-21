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
 * RandomWalkDisplacement.h
 *
 *  Created on: Nov 20, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_RANDOMWALKDISPLACEMENT_H_
#define INCLUDE_RANDOMWALKDISPLACEMENT_H_

#include <Displace.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

class RandomWalkDisplacement: public Displace {

public:
	RandomWalkDisplacement(Map* map, Clock* clk, double speed);
	virtual Point* generateNewLocation(Point* p) override;
	virtual ~RandomWalkDisplacement();
};

#endif /* INCLUDE_RANDOMWALKDISPLACEMENT_H_ */
