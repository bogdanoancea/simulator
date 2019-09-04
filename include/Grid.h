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
 * This class implements a gird  of rectangular tiles overlapped on the map of the simulation. This grid is used to compute
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
	 * @return a string representation of an object of type Grid. This is useful to write a textual description of the grid in a file
	 * for later processing.
	 */
	string toString() const;

	/**
	 * Returns the tile index on X axis that contains a given point in space, specified by @param p.
	 * @param p a pointer to the point for which we need the tile index.
	 * @return the tile index on X axis that contains the point specified by @param p, i.e. a number between 0 and getNoTilesX() - 1.
	 */
	unsigned long getTileIndexX(const Point* p) const;

	/**
	 * Returns the tile index on Y axis that contains a given point in space, specified by @param p.
	 * @param p the point in space for which we need the tile index.
	 * @return the tile index on Y axis that contains the point specified by @param p, i.e. a number between 0 and getNoTilesY() - 1.
	 */
	unsigned long getTileIndexY(const Point* p) const;

	/**
	 * Computes the total number of tiles in the grid.
	 * @return the total number of tiles in the grid.
	 */
	const unsigned long getNoTiles() const;

	/**
	 * Computes the posterior probability of a mobile device to be in a tile of the Grid according to the method
	 * described in he paper "Deriving geographic location of mobile devices from network data"
	 * by Martijn Tennekes, Yvonne A.P.M. Gootzen, Shan H. Shah.
	 * @param t the time instant when the posterior localization probability is computed.
	 * @param m a pointer to a MobilePhone object for which the posterior localization probability is computed.
	 * @param data a vector of AntennaInfo objects generated and recorded by each antenna during the simulation.
	 * It contains the events recorder by each antenna during the simulation.
	 * @param it an iterator to access all objects of type Antenna from the AgentsCollection container.
	 * @param prior is used to set the method of computing the prior probabilities. It could take 3 values:
	 * PriorType::UNIFORM, PriorType::NETWORK or PriorType::REGISTER. Currently only
	 * UNIFORM and NETWORK methods are implemented.
	 * @return a vector with the posterior probability of the mobile phone given by @param m to be localized in a tile. The index of a value
	 * in this vector indicates the corresponding tile index. The size of this vector is equat to the total number of tiles in the Grid.
	 */
	vector<double> computeProbability(unsigned long t, MobilePhone* m, vector<AntennaInfo>& data, pair<um_iterator, um_iterator> it, PriorType prior) const;

	/**
	 * Computes the coordinates of the tile center given by its index in the grid.
	 * @param tileIndex the tile index.
	 * @return the coordinates of the center of the tile.
	 */
	Coordinate getTileCenter(unsigned long tileIndex) const;

	/**
	 * Computes the tile index of the tile that contains the Point indicated by @param p.
	 * @param p a pointer to a Point object.
	 * @return the tile index of the tile that contains the Point indicated by @param p.
	 */
	unsigned long getTileNo(const Point* p) const;

	unsigned long getTileIndexX(double x) const;
	unsigned long getTileIndexY(double y) const;

	/**
	 * Computes the tile index of the tile that contains a point with coordinates indicated by @param x and @param y.
	 * @param x x coordinate of a location.
	 * @param y y coordinate of a location.
	 * @return the tile index of the tile that contains a point with coordinates indicated by @param x and @param y.
	 */
	unsigned long getTileNo(double x, double y) const;

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
	vector<double> useNetworkPrior(unsigned long t, bool connected, vector<AntennaInfo>::iterator ai, pair<um_iterator, um_iterator> antennas_iterator) const;
	vector<double> useUniformPrior(unsigned long t, bool connected, vector<AntennaInfo>::iterator ai, pair<um_iterator, um_iterator> antennas_iterator) const;

};

#endif /* GRID_H_ */
