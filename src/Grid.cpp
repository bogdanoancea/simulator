/*
 * Data simulator for mobile phone network events
 *
 * Grid.cpp
 *
 *  Created on: May 7, 2019
 *      Author: Bogdan Oancea
 */

#include <Grid.h>

Grid::Grid(double xOrig, double yOrig, double xdim, double ydim, unsigned long noTilesX, unsigned long noTilesY) :
		m_xOrigin { xOrig }, m_yOrigin { yOrig }, m_xDim { xdim }, m_yDim { ydim }, m_noTilesX { noTilesX }, m_noTilesY { noTilesY } {
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

unsigned long Grid::getNoTilesX() const {
	return (m_noTilesX);
}

unsigned long Grid::getNoTilesY() const {
	return (m_noTilesY);
}

double Grid::getXDim() const {
	return (m_xDim);
}

double Grid::getXOrigin() const {
	return (m_xOrigin);
}

double Grid::getYDim() const {
	return (m_yDim);
}

double Grid::getYOrigin() const {
	return (m_yOrigin);
}

unsigned long Grid::getTileIndexX(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileIndexY(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileOrigX(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileOrigY(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileCenterX(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileCenterY(Point* p) {
	unsigned long result = -1;

	return (result);
}
