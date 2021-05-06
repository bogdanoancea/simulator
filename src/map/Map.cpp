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
 *  Created on: Dec 19, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/Geometry.h>
#include <geos/version.h>
#include <map/Map.h>
#include <cmath>
#include <iostream>
#include <limits>
#include <stdexcept>


Map::Map(string mapFileName) {
	m_mapFileName = mapFileName;
	m_grid = nullptr;
}

Map::~Map(){
	if (m_grid != nullptr)
		delete m_grid;
}

void Map::addGrid(double dimTileX, double dimTileY) {

#if GEOS_VERSION_MAJOR >= 3
#if GEOS_VERSION_MINOR > 7
	CoordinateSequence* seq = getEnvelope()->getCoordinates().release();
#else
	CoordinateSequence* seq = getEnvelope()->getCoordinates();
#endif
#else
	throw std::runtime_error("unsupported geos version");
#endif
	double minX, minY, maxX, maxY;
	minX = minY = numeric_limits<double>::max();
	maxX = maxY = numeric_limits<double>::min();
	for (size_t i = 0; i < seq->size(); i++) {
		double x = seq->getX(i);
		double y = seq->getY(i);
		if (x > maxX)
			maxX = x;
		if (y > maxY)
			maxY = y;
		if (x < minX)
			minX = x;
		if (y < minY)
			minY = y;
	}
	unsigned long noTilesX = ceil((maxX - minX) / dimTileX);
	unsigned long noTilesY = ceil((maxY - minY) / dimTileY);
	m_grid = new Grid(minX, minY, dimTileX, dimTileY, noTilesX, noTilesY);
}

unsigned long Map::getNoTilesX() const {
	if(m_grid)
		return m_grid->getNoTilesX();
	else
		throw runtime_error("no grid defined");
}

unsigned long Map::getNoTilesY() const {
	if(m_grid)
		return m_grid->getNoTilesY();
	else
		throw runtime_error("no grid defined");
}

double Map::getXTileDim() const {
	if(m_grid)
		return m_grid->getXTileDim();
	else
		throw runtime_error("no grid defined");
}

double Map::getYTileDim() const {
	if(m_grid)
		return m_grid->getYTileDim();
	else
		throw runtime_error("no grid defined");
}

double Map::getXOrigin() const {
	if(m_grid)
		return m_grid->getXOrigin();
	else
		throw runtime_error("no grid defined");
}

double Map::getYOrigin() const {
	if(m_grid)
		return m_grid->getYOrigin();
	else
		throw runtime_error("no grid defined");
}


const unsigned long Map::getNoTiles() const {
	if(m_grid)
		return m_grid->getNoTiles();
	else
		throw runtime_error("no grid defined");
}

Coordinate Map::getTileCenter(unsigned long tileIndex) const {
	if(m_grid)
		return m_grid->getTileCenter(tileIndex);
	else
		throw runtime_error("no grid defined");
}


unsigned long Map::getTileNo(const Point* p) const {
	if(m_grid)
		return m_grid->getTileNo(p);
	else
		throw runtime_error("no grid defined");

}

unsigned long Map::getTileNo(double x, double y) const {
	if(m_grid)
		return m_grid->getTileNo(x, y);
	else
		throw runtime_error("no grid defined");

}

void Map::dumpGrid(const string& gridFileName) const {
	if(m_grid)
		return m_grid->dumpGrid(gridFileName);
	else
		throw runtime_error("no grid defined");
}


Coordinate* Map::getTileCenters() const {
	if(m_grid)
		return m_grid->getTileCenters();
	else
		throw runtime_error("no grid defined");

}

bool Map::hasGrid() const {
	if(m_grid)
		return true;
	else
		return false;
}
