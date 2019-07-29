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
#include <fstream>
#include <sstream>


using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::io;

Map::Map() {
	// Define a precision model using 0,0 as the reference origin
	// and 2.0 as coordinates scale.

	PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
	// Initialize global factory with defined PrecisionModel
	// and a SRID of -1 (undefined).
	m_globalFactory = GeometryFactory::create(pm, -1);

	// We do not need PrecisionMode object anymore, it has
	// been copied to global_factory private storage
	delete pm;
	m_boundary = nullptr;
	m_grid = nullptr;
}

Map::Map(double llx, double llY, double width, double height) {
	PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
	// Initialize global factory with defined PrecisionModel
	// and a SRID of -1 (undefined).
	m_globalFactory = GeometryFactory::create(pm, -1);

	// We do not need PrecisionMode object anymore, it has
	// been copied to global_factory private storage
	delete pm;

	m_boundary = (Geometry*) create_rectangle(0, 0, 10, 10);
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
	//cout << "mapa :" << buffer.str() << endl;
	m_boundary = reader.read(buffer.str());
	try {
		wktFile.close();
	} catch (ofstream::failure& e) {
		cerr << "Error closing map file!" << endl;
		throw e;
	}
	m_grid = nullptr;
//	Geometry* bbox = m_boundary->getEnvelope();
//	CoordinateSequence* seq = bbox->getCoordinates();
//	double minX, minY, maxX, maxY;
//	minX = minY = numeric_limits<double>::max();
//	maxX = maxY = numeric_limits<double>::min();
//	for (size_t i = 0; i < seq->size(); i++) {
//		double x = seq->getX(i);
//		double y = seq->getY(i);
//		if (x > maxX)
//			maxX = x;
//		if (y > maxY)
//			maxY = y;
//		if (x < minX)
//			minX = x;
//		if (y < minY)
//			minY = y;
//
//	}
//	unsigned long noTilesX = (maxX - minX) / dimTileX;
//	unsigned long noTilesY = (maxY - minY) / dimTileY;
//
//	//Grid g(minX, minY, dimTileX, dimTileY, w.getGridTilesX(), w.getGridTilesX());
//	m_grid(minX, minY, dimTileX, dimTileY, noTilesX, noTilesY);
}

Map::~Map() {
	if (m_boundary != nullptr)
		delete m_boundary;
	if(m_grid != nullptr)
		delete m_grid;
}

const GeometryFactory::Ptr& Map::getGlobalFactory() const {
	return (m_globalFactory);
}

Polygon*
Map::create_rectangle(double llX, double llY, double width, double height) {
	geos::util::GeometricShapeFactory shapefactory(m_globalFactory.get());

	shapefactory.setBase(Coordinate(llX, llY));
	shapefactory.setHeight(height);
	shapefactory.setWidth(width);
	shapefactory.setNumPoints(4); // we don't need more then 4 points for a rectangle...
// can use setSize for a square
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
	unsigned long noTilesX = (maxX - minX) / dimTileX;
	unsigned long noTilesY = (maxY - minY) / dimTileY;

	//Grid g(minX, minY, dimTileX, dimTileY, w.getGridTilesX(), w.getGridTilesX());
	m_grid = new Grid(minX, minY, dimTileX, dimTileY, noTilesX, noTilesY);

}

