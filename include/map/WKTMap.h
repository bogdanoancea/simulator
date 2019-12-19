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
 * Map.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef WKTMAP_H
#define WKTMAP_H

#include <geos/geom/GeometryFactory.h>
#include "map/Map.h"

using namespace std;
using namespace geos;
using namespace geos::geom;

/**
 * This is the map where the simulation takes place. It could be a simple rectangle or any kind of geometry object(s)
 * read from a wkt file. The map has a boundary that is implemented as a Geometry object, a factory object of GeometryFactory type
 * used to create other objects. All geometric and geographic features of the simulator uses the GEOS library.
 * GEOS is an open source C++ library which is just a port to C++ of the well known Java Topology Suite.
 */

class WKTMap: public Map {
public:
	/**
	 * Builds a map reading it from a .wkt file.
	 * @param wktFile the name of the .wkt file that contains the description of the map.
	 * Currently, the first row of this file should contain the external boundary geometry of the map.
	 */
	WKTMap(string wktFile);

	/**
	 * Default destructor
	 */
	virtual ~WKTMap();

	/**
	 * Returns a pointer to the GeometryFactory object which is a factory object used to create other geometric objects. The GEOS
	 * library allows users to create geometric objects only using this factory, all the constructors are made private.
	 * @return a pointer to the GeometryFactory object used to create other geometric objects.
	 */
	const GeometryFactory::Ptr& getGlobalFactory() const override;

	/**
	 * Returns a pointer to the Geometry object that represents the external boundary of the map.
	 * @return a pointer to the Geometry object that represents the external boundary of the map.
	 */
	virtual Geometry* getBoundary() const override;

	virtual Geometry* getEnvelope() const override;

private:
	GeometryFactory::Ptr m_globalFactory;
	Geometry* m_boundary;
};

#endif // MAP_H




