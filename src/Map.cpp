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
	m_boundary = reader.read(buffer.str());
	try {
		wktFile.close();
	} catch (ofstream::failure& e) {
		cerr << "Error closing map file!" << endl;
		throw e;
	}
}

Map::~Map() {
	if(m_boundary != nullptr)
	delete m_boundary;
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

