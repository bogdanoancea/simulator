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

using namespace std;
using namespace geos;
using namespace geos::geom;
/*
 *
 */
class Grid {
	public:
		Grid(Map* map, double xOrig, double yOrig, double xTiledim, double yTiledim, unsigned long noTilesX, unsigned long noTilesY);
		virtual ~Grid();

		unsigned long getNoTilesX() const;
		unsigned long getNoTilesY() const;
		double getXTileDim() const;
		double getXOrigin() const;
		double getYTileDim() const;
		double getYOrigin() const;

		string toString() const;


		unsigned long getTileIndexX(Point* p);
		unsigned long getTileIndexY(Point* p);
		unsigned long getTileCenterX(Point* p);
		unsigned long getTileCenterY(Point* p);


		unsigned long getNoTiles() const;
		double computeProbability(unsigned long t, unsigned long tileIndex, MobilePhone* m, vector<AntennaInfo>& data,
				std::pair<um_iterator, um_iterator>);
		Coordinate getTileCenter(unsigned long tileIndex);

	private:
		double m_xOrigin;
		double m_yOrigin;
		double m_xTileDim;
		double m_yTileDim;
		unsigned long m_noTilesX;
		unsigned long m_noTilesY;
		Map* m_map;
};

#endif /* GRID_H_ */
