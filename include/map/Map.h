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
#ifndef MAP_H
#define MAP_H

#include <geos/geom/GeometryFactory.h>
#include <map/Grid.h>
#include <string>


using namespace std;
using namespace geos;
using namespace geos::geom;


class Map {

public:

	virtual ~Map();

	virtual Geometry* getBoundary() const = 0;
	virtual Geometry* getEnvelope() const = 0;

	//temporar
		const virtual GeometryFactory::Ptr& getGlobalFactory() const = 0;

	virtual void addGrid(double dimTileX, double dimTileY);
	virtual unsigned long getNoTilesX() const;
	virtual unsigned long getNoTilesY() const ;
	virtual double getXTileDim() const;
	virtual double getYTileDim() const;
	virtual double getXOrigin() const;
	virtual double getYOrigin() const;
		const virtual unsigned long getNoTiles() const;
	virtual Coordinate getTileCenter(unsigned long tileIndex) const;
	virtual unsigned long getTileNo(const Point* p) const;
	virtual unsigned long getTileNo(double x, double y) const;
	virtual void dumpGrid(const string& gridFileName) const;
	virtual Coordinate* getTileCenters() const;
	virtual bool hasGrid() const;

protected:
	Map(string mapFileName);

	Grid* m_grid;
	string m_mapFileName;
};

#endif /* MAP_H */
