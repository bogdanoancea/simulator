/*
 * Data simulator for mobile phone network events
 *
 * Grid.h
 *
 *  Created on: May 7, 2019
 *      Author: Bogdan Oancea
 */

#ifndef GRID_H_
#define GRID_H_

#include <string>
#include <iostream>
#include <vector>
#include <geos/geom/Coordinate.h>
#include <MobilePhone.h>
#include <AntennaInfo.h>
#include <Agent.h>
#include <AgentsCollection.h>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <PriorType.h>

using namespace std;
using namespace geos;
using namespace geos::geom;

/**
 * We overlap a grid of rectangular tiles over the map of the simulation and use this grid to compute the "observed" location
 * of a mobile phone. This means that we compute the probability of a mobile device to be in a specific tile of the grid using the
 * data recorded by each antenna during the simulation. A finer grid will give a more accurate location but the computational cost
 * increase when the size of the tiles decrease.
 * The tiles of the grid are indexed starting with 0 for the tile in the bottom left corner of the
 * grid in a row-major ordering. The last tile, with the biggest index,
 * is the tile in the upper-right corner of the grid.
 */
class Grid {
public:

	/**
	 * Constructor of the class. Build an object with the specified parameters.
	 * @param map a pointer to the Map object used in the simulation
	 * @param xOrig the x coordinate of the origin of the grid (i.e. the x coordinate of the bottom left corner of the grid)
	 * @param yOrig the y coordinate of the origin of the grid (i.e. the y coordinate of the bottom left corner of the grid)
	 * @param xTiledim the dimension of a tile on X axis direction
	 * @param yTiledim the dimension of a tile on Y axis direction
	 * @param noTilesX the number of tiles on X axis
	 * @param noTilesY the number of tiles on Y axis
	 */
//	Grid(Map* map, double xOrig, double yOrig, double xTiledim, double yTiledim,
//			unsigned long noTilesX, unsigned long noTilesY);

	Grid(){};
	Grid(double xOrig, double yOrig, double xTiledim, double yTiledim,
				unsigned long noTilesX, unsigned long noTilesY);

	/**
	 * Destructor
	 */
	virtual ~Grid();

	/**
	 * @return the number of tiles in the grid on X axis direction
	 */
	unsigned long getNoTilesX() const;

	/**
	 * @return the number of tiles in the grid on Y axis direction
	 */
	unsigned long getNoTilesY() const;

	/**
	 * @return the dimension of a tile on X axis direction
	 */
	double getXTileDim() const;

	/**
	 * @return the dimension of a tile on Y axis direction
	 */
	double getYTileDim() const;

	/**
	 * @return the x coordinate of the origin of the grid (i.e. the x coordinate of the bottom left corner of the grid)
	 */
	double getXOrigin() const;

	/**
	 *
	 * @return the y coordinate of the origin of the grid (i.e. the y coordinate of the bottom left corner of the grid)
	 */
	double getYOrigin() const;

	/**
	 * @return a string representation of an abject of type Grid
	 */
	string toString() const;

	/**
	 * Returns the tile index on X axis that contains a given point in space.
	 * @param p the point in space for which we need the tile index.
	 * @return the tile index on X axis that contains a given point in space, i.e. a number between 0 and getNoTilesX() - 1.
	 */
	unsigned long getTileIndexX(const Point* p) const;

	/**
	 * Returns the tile index on Y axis that contains a given point in space.
	 * @param p the point in space for which we need the tile index.
	 * @return the tile index on Y axis that contains a given point in space, i.e. a number between 0 and getNoTilesY() - 1.
	 */
	unsigned long getTileIndexY(const Point* p) const;

	/**
	 * Computes the center of a tile where a given point is located
	 * @param p the location for which we want to find the tile where it is placed and compute the center of this tile.
	 * @return the X coordinate of the center of a tile where a given point is located
	 */
	unsigned long getTileCenterX(Point* p);

	/**
	 * Computes the center of a tile where a given point is located
	 * @param p the location for which we want to find the tile where it is placed and compute the center of this tile.
	 * @return the Y coordinate of the center of a tile where a given point is located
	 */
	unsigned long getTileCenterY(Point* p);

	/**
	 * Computes the total number of tiles in the grid
	 * @return the total number of tiles in the grid
	 */
	unsigned long getNoTiles() const;

	/**
	 * Computes the posterior probability of a mobile device to be in a tile according to the method
	 * described in he paper "Deriving geographic location of mobile devices from network data"
	 * by Martijn Tennekes, Yvonne A.P.M. Gootzen, Shan H. Shah.
	 * @param t the time instant when we want to compute the posterior localization probability.
	 * @param m a pointer to a MobilePhone object for which we want to compute the posterior localization probability.
	 * @param data a vector of AntennaInfo objects generated and recorder by each antenna during the simulation.
	 * @param it an iterator to access all agents of type Antenna from the AgentsCollection container
	 * @param prior set the way prior probabilities are computed: uniform or network based
	 * @return a vector with the posterior probability of the mobile phone m to be localized in the tile. Probabilities are
	 * placed in vector according to their tile index.
	 */
	vector<double> computeProbability(unsigned long t, MobilePhone* m,
			vector<AntennaInfo>& data, pair<um_iterator, um_iterator> it,
			PriorType prior) const;

	/**
	 * Computes the coordinates of the tile center given by its index in the grid
	 * @param tileIndex the tile index
	 * @return the coordinates of the center of the tile.
	 */
	Coordinate getTileCenter(unsigned long tileIndex)const ;

	unsigned long getTileNo(const Point* p) const;

private:
	//Map* m_map;
	double m_xOrigin;
	double m_yOrigin;
	double m_xTileDim;
	double m_yTileDim;
	unsigned long m_noTilesX;
	unsigned long m_noTilesY;
	Coordinate* m_tileCenters;


	Coordinate* computeTileCenters();
	vector<double> useNetworkPrior(unsigned long t, bool connected,
			vector<AntennaInfo>::iterator ai,
			pair<um_iterator, um_iterator> antennas_iterator) const;

	vector<double> useUniformPrior(unsigned long t, bool connected,
			vector<AntennaInfo>::iterator ai,
			pair<um_iterator, um_iterator> antennas_iterator) const;

};

#endif /* GRID_H_ */
