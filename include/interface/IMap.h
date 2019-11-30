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
 * IMap.h
 *
 *  Created on: Nov 27, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */
#ifndef INCLUDE_INTERFACE_IMAP_H_
#define INCLUDE_INTERFACE_IMAP_H_

#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <string>
#include <geos/geom/Point.h>

using namespace std;
using namespace geos;
using namespace geos::geom;


class IMap {

public:

	virtual Geometry* getBoundary() const = 0;
	virtual void addGrid(double dimTileX, double dimTileY) = 0;

	virtual unsigned long getNoTilesX() const = 0;
	virtual unsigned long getNoTilesY() const = 0;
	virtual double getXTileDim() const = 0;
	virtual double getYTileDim() const;
	virtual double getXOrigin() const = 0;
	virtual double getYOrigin() const = 0;
	virtual const unsigned long getNoTiles() const = 0;
	virtual Coordinate getTileCenter(unsigned long tileIndex) const = 0;
	virtual unsigned long getTileNo(const Point* p) const = 0;
	virtual unsigned long getTileNo(double x, double y) const = 0;
	virtual void dumpGrid(const string& gridFileName) const = 0;
	virtual Coordinate* getTileCenters() const = 0;

};

#endif /* INCLUDE_INTERFACE_IMAP_H_ */
