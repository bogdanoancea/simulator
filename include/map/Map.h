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
#include <string>
#include <map/Grid.h>

using namespace std;
using namespace geos;
using namespace geos::geom;

/**
 * This is an abstract the class defining the interface of any implementing class that will be used as the map of the simulation.
 * Currently there is only one implementation, a WKTMap class, which reads the external boundary from a WKT (Well Known Text) string
 * stored in a file. This file is provided in the command line with "-m" option.
 * The map has a boundary that is implemented as a Geometry object, a factory object of GeometryFactory type
 * used to create other objects. It also embeds a grid of rectangular tiles that covers the whole map. These tiles are used later for computing
 * the localization probability for each mobile device. All the methods requesting the grid parameters are delegated to the Grid object embeded in the Map class.
 * All geometric and geographic features of the simulator use the GEOS++ library.
 * GEOS++ is an open source C++ library which is just a port to C++ of the well known Java Topology Suite.
 */
class Map {

public:

	/**
	 * Default destructor.
	 */
	virtual ~Map();

	/**
	 * Returns a pointer to a Geometry object that represents the external boundary of the map.
	 * @return a pointer to a Geometry object that represents the external boundary of the map.
	 */
	virtual Geometry* getBoundary() const = 0;

	/**
	 * Returns the bounding box of the map.
	 * @return the bounding box of the map.
	 */
	virtual Geometry* getEnvelope() const = 0;

	/**
	 * It returns a pointer to the GeometryFactory of the GEOS++ library needed to create Geometry objects.
	 * @return a pointer to the GeometryFactory of the GEOS++ library needed to create Geometry objects.
	 */
	const virtual GeometryFactory::Ptr& getGlobalFactory() const = 0;

	/**
	 * Creates a Grid object with the dimensions of tiles specified as parameters. It computes to necessary number of tiles such that the Grid covers
	 * the bounding box of the map. The origin point of the grid (the left bottom point) is set to the minimum values for x and y coordinates
	 * of the bounding box of the Map. A pointer to the Grid object is then stored as a member of the Map class.
	 * @param dimTileX the dimension of a tile on OX axis.
	 * @param dimTileY the dimension of a grid on OY axis.
	 */
	virtual void addGrid(double dimTileX, double dimTileY);

	/**
	 * Returns the number of tiles of the grid on OX axis.
	 * @return the number of tiles of the grid on OX axis.
	 */
	virtual unsigned long getNoTilesX() const;

	/**
	 * Returns the number of tiles of the grid on OY axis.
	 * @return the number of tiles of the grid on OY axis.
	 */
	virtual unsigned long getNoTilesY() const;

	/**
	 * Returns the dimension of tile on OX axis.
	 * @return the dimension of tile on OX axis.
	 */
	virtual double getXTileDim() const;

	/**
	 * Returns the dimension of tile on OY axis.
	 * @return the dimension of tile on OY axis.
	 * @return
	 */
	virtual double getYTileDim() const;

	/**
	 * Returns the x coordinate of the origin of the grid.
	 * @return the x coordinate of the origin of the grid.
	 */
	virtual double getXOrigin() const;

	/**
	 * Returns the y coordinate of the origin of the grid.
	 * @return the y coordinate of the origin of the grid.
	 */
	virtual double getYOrigin() const;

	/**
	 * Returns the total number of tiles in the grid.
	 * @return the total number of tiles in the grid.
	 */
	const virtual unsigned long getNoTiles() const;

	/**
	 * Given a tile index, this method computes the center of the tile.
	 * @param tileIndex the tile index. Index 0 is assigned to the bottom left tile and the tiles are numbered
	 * using a row-major order.
	 * @return the center of the tile with the index provided through the \a tileIndex parameter.
	 */
	virtual Coordinate getTileCenter(unsigned long tileIndex) const;

	/**
	 * Given a position on the map specified by a Point object, this method computes the tile number where this position falls.
	 * @param p a position on the map.
	 * @return the tile number where the position specified by \a p falls.
	 */
	virtual unsigned long getTileNo(const Point* p) const;

	/**
	 * Given a position on the map specified by a x and y coordinates, this method computes the tile number where this position falls.
	 * @param x the x coordinate of the position.
	 * @param y the x coordinate of the position.
	 * @return the tile number where the position specified by \a x and \a y falls.
	 */
	virtual unsigned long getTileNo(double x, double y) const;

	/**
	 * Writes the description of the grid to a file. This name of the file is read from the simulation configuration file. An example file
	 * is given here:
	 *
	 *  \code
	 * Origin X,Origin Y,X Tile Dim,Y Tile Dim,No Tiles X,No Tiles Y
	 * 0.000000,0.000000,250.000000,250.000000,40,40
	 * \endcode
	 * @param gridFileName the name of the file where the description of the grid is written.
	 */
	virtual void dumpGrid(const string& gridFileName) const;

	/**
	 * Returns an array with the coordinates of the centers of all tiles in the grid.
	 * @return an array of Coordinate objects with the coordinates of the centers of all tiles in the grid.
	 */
	virtual Coordinate* getTileCenters() const;

	/**
	 * Returns true if the grid is set, false otherwise. This method is needed because the Grid object is not created by the constructor of this class
	 * but it is set a later moment, after the parameters of the grid are read from the simulation configuration file.
	 * @return
	 */
	virtual bool hasGrid() const;

protected:
	Map(string mapFileName);

	Grid* m_grid;
	string m_mapFileName;
};

#endif /* MAP_H */
