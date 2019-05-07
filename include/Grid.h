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

/*
 *
 */
class Grid {
	public:
		Grid(double xOrig, double yOrig, double xdim, double ydim);
		virtual ~Grid();

	private:
		double m_xOrigin;
		double m_yOrigin;
		double m_xDim;
		double m_yDim;
};

#endif /* GRID_H_ */
