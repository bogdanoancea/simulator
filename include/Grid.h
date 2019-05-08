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

#include <geos/geom/Polygon.h>

using namespace geos;
using namespace geos::geom;
/*
 *
 */
class Grid {
	public:
		Grid(double xOrig, double yOrig, double xdim, double ydim, unsigned long noTilesX, unsigned long noTilesY);
		virtual ~Grid();
		unsigned long getNoTilesX() const;
		unsigned long getNoTilesY() const;
		double getXDim() const;
		double getXOrigin() const;
		double getYDim() const;
		double getYOrigin() const;

		unsigned long getTileIndexX(Point* p);
		unsigned long getTileIndexY(Point* p);
		unsigned long getTileOrigX(Point* p);
		unsigned long getTileOrigY(Point* p);
		unsigned long getTileCenterX(Point* p);
		unsigned long getTileCenterY(Point* p);


	private:
		double m_xOrigin;
		double m_yOrigin;
		double m_xDim;
		double m_yDim;
		unsigned long m_noTilesX;
		unsigned long m_noTilesY;
};

#endif /* GRID_H_ */
