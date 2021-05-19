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
 * Grid.h
 *
 *  Created on: May 7, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef GRID_H_
#define GRID_H_

#include <string>
#include <iostream>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Point.h>

using namespace std;
using namespace geos;
using namespace geos::geom;

/**
 * This class implements a grid  of rectangular tiles overlapped on the map of the simulation. This grid is used to compute
 * the "observed" location of a mobile phone. This means that we compute the probability of a mobile device to be in a
 * specific tile of the grid using the data recorded by each antenna during the simulation. A finer grid will give a
 * more accurate location but the computational cost increase when the size of the tiles decrease.
 * The tiles of the grid are indexed starting with 0 for the tile in the bottom left corner of the
 * grid in a row-major ordering. The last tile, with the biggest index, is the tile in the upper-right corner of the grid.
 */
class Grid {
public:

	/**
	 * Constructor of the class. Build a Grid object with the specified parameters.
	 * @param xOrig the x coordinate of the origin of the grid (i.e. the x coordinate of the bottom left corner of the grid).
	 * @param yOrig the y coordinate of the origin of the grid (i.e. the y coordinate of the bottom left corner of the grid).
	 * @param xTiledim the dimension of a tile on X axis.
	 * @param yTiledim the dimension of a tile on Y axis.
	 * @param noTilesX the number of tiles on X axis.
	 * @param noTilesY the number of tiles on Y axis.
	 */
	Grid(double xOrig, double yOrig, double xTiledim, double yTiledim, unsigned long noTilesX, unsigned long noTilesY);

	/**
	 * Default destructor.
	 */
	virtual ~Grid();

	/**
	 * @return the number of tiles of the grid on X axis direction.
	 */
	unsigned long getNoTilesX() const;

	/**
	 * @return the number of tiles of the grid on Y axis direction.
	 */
	unsigned long getNoTilesY() const;

	/**
	 * @return the dimension of a tile on X axis direction.
	 */
	double getXTileDim() const;

	/**
	 * @return the dimension of a tile on Y axis direction.
	 */
	double getYTileDim() const;

	/**
	 * @return the x coordinate of the origin of the grid (i.e. the x coordinate of the bottom left corner of the grid).
	 */
	double getXOrigin() const;

	/**
	 *
	 * @return the y coordinate of the origin of the grid (i.e. the y coordinate of the bottom left corner of the grid).
	 */
	double getYOrigin() const;


	/**
	 * Computes the total number of tiles in the grid.
	 * @return the total number of tiles in the grid.
	 */
	const unsigned long getNoTiles() const;


	/**
	 * Computes the coordinates of the tile center given by its index in the grid.
	 * @param tileIndex the tile index.
	 * @return the coordinates of the center of the tile.
	 */
	Coordinate getTileCenter(unsigned long tileIndex) const;

	/**
	 * Computes the tile index of the tile that contains the Point indicated by p.
	 * @param p a pointer to a Point object.
	 * @return the tile index of the tile that contains the Point indicated by p.
	 */
	unsigned long getTileNo(const Point* p) const;


	/**
	 * Computes the tile index of the tile that contains a point with coordinates indicated by x and y.
	 * @param x x coordinate of a location.
	 * @param y y coordinate of a location.
	 * @return the tile index of the tile that contains a point with coordinates indicated by x and y.
	 */
	unsigned long getTileNo(double x, double y) const;

	/**
	 * Writes the grid description in a .csv file for later processing.
	 * @param gridFileName the name of the output file.
	 */
	void dumpGrid(const string& gridFileName) const;


	/**
	 * Returns a vector containing the coordinates of the tile centers.
	 * @return a vector containing the coordinates of the tile centers.
	 */
	Coordinate* getTileCenters() const;

private:
	double m_xOrigin;
	double m_yOrigin;
	double m_xTileDim;
	double m_yTileDim;
	unsigned long m_noTilesX;
	unsigned long m_noTilesY;
	Coordinate* m_tileCenters;


	unsigned long getTileIndexX(double x) const;
	unsigned long getTileIndexY(double y) const;

	/**
	 * Returns the tile index on X axis that contains a given point in space, specified by p.
	 * @param p a pointer to the point for which we need the tile index.
	 * @return the tile index on X axis that contains the point specified by p, i.e. a number between 0 and getNoTilesX() - 1.
	 */
	unsigned long getTileIndexX(const Point* p) const;

	/**
	 * Returns the tile index on Y axis that contains a given point in space, specified by p.
	 * @param p the point in space for which we need the tile index.
	 * @return the tile index on Y axis that contains the point specified by p, i.e. a number between 0 and getNoTilesY() - 1.
	 */
	unsigned long getTileIndexY(const Point* p) const;

	/**
	 * @return a string representation of a \code{Grid} object. This is useful to write a textual description of the grid to a file
	 * for later processing.
	 */
	string toString() const;

	Coordinate* computeTileCenters();


};

#endif /* GRID_H_ */
