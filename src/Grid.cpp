/*
 * Data simulator for mobile phone network events
 *
 * Grid.cpp
 *
 *  Created on: May 7, 2019
 *      Author: Bogdan Oancea
 */

#include <Grid.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>


using namespace std;

Grid::Grid(double xOrig, double yOrig, double xTiledim, double yTiledim, unsigned long noTilesX, unsigned long noTilesY) :
		m_xOrigin { xOrig }, m_yOrigin { yOrig }, m_xTileDim { xTiledim }, m_yTileDim { yTiledim }, m_noTilesX { noTilesX }, m_noTilesY {
				noTilesY } {
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

string Grid::toString() const {
	ostringstream ss;
	ss << left << setw(15) << "Origin X" << setw(15) << m_xOrigin << setw(15) << "Origin Y" << setw(15) << m_yOrigin << setw(15)
			<< "x tile dim"
			<< setw(15) << m_xTileDim << setw(15) << "y tile dim" << setw(15) << m_yTileDim << setw(15) << "no tiles x" << setw(15)
			<< m_noTilesX << setw(15) << "no tiles y" << setw(15) << m_noTilesY << endl;
	return (ss.str());
}

unsigned long Grid::getNoTiles() const {
	return (m_noTilesX * m_noTilesY);
}

unsigned long Grid::getNoTilesX() const {
	return (m_noTilesX);
}

unsigned long Grid::getNoTilesY() const {
	return (m_noTilesY);
}

double Grid::getXTileDim() const {
	return (m_xTileDim);
}

double Grid::getXOrigin() const {
	return (m_xOrigin);
}

double Grid::getYTileDim() const {
	return (m_yTileDim);
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

unsigned long Grid::getTileCenterX(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileCenterY(Point* p) {
	unsigned long result = -1;

	return (result);
}
