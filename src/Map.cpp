/*
 * Data simulator for mobile phone network events
 *
 * Map.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <geos/geom/Coordinate.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/util/GeometricShapeFactory.h>
#include <Map.h>
#include<Grid.h>
#include <memory>
#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::io;

Map::Map() {
	PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
	m_globalFactory = GeometryFactory::create(pm, -1);
	delete pm;
	m_boundary = nullptr;
	m_grid = nullptr;
}

Map::Map(double llx, double llY, double width, double height) {
	PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
	m_globalFactory = GeometryFactory::create(pm, -1);
	delete pm;
	m_boundary = (Geometry*) create_rectangle(llx, lly, width, height);
	m_grid = nullptr;
}

Map::Map(string wktFileName) {
	PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
	m_globalFactory = GeometryFactory::create(pm, -1);
	delete pm;

	WKTReader reader(*m_globalFactory);
	ifstream wktFile;
	try {
		wktFile.open(wktFileName, ios::in);
	} catch (ofstream::failure& e) {
		cerr << "Error opening map file!" << endl;
		throw e;
	}

	stringstream buffer;
	buffer << wktFile.rdbuf();
	m_boundary = reader.read(buffer.str());
	try {
		wktFile.close();
	} catch (ofstream::failure& e) {
		cerr << "Error closing map file!" << endl;
		throw e;
	}
	m_grid = nullptr;
}

Map::~Map() {
	if (m_boundary != nullptr)
		delete m_boundary;
	if (m_grid != nullptr)
		delete m_grid;
}

const GeometryFactory::Ptr& Map::getGlobalFactory() const {
	return (m_globalFactory);
}

Polygon* Map::create_rectangle(double llX, double llY, double width, double height) {
	geos::util::GeometricShapeFactory shapefactory(m_globalFactory.get());
	shapefactory.setBase(Coordinate(llX, llY));
	shapefactory.setHeight(height);
	shapefactory.setWidth(width);
	shapefactory.setNumPoints(4);
	return (shapefactory.createRectangle());
}

const Grid* Map::getGrid() const {
	return m_grid;
}

void Map::addGrid(double dimTileX, double dimTileY) {
	Geometry* bbox = m_boundary->getEnvelope();
	CoordinateSequence* seq = bbox->getCoordinates();
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

Geometry* Map::getBoundary() const {
	return (m_boundary);
}

void Map::setBoundary(Geometry* boundary) {
	m_boundary = boundary;
}
