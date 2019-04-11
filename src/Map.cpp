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


using namespace std;
using namespace geos;
using namespace geos::geom;



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

}

Map::Map(double llx, double llY, double width, double height) :
		Map() {
	PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
	// Initialize global factory with defined PrecisionModel
	// and a SRID of -1 (undefined).
	m_globalFactory = GeometryFactory::create(pm, -1);

	// We do not need PrecisionMode object anymore, it has
	// been copied to global_factory private storage
	delete pm;

	m_boundary = (geos::geom::Geometry*) create_rectangle(0, 0, 10, 10);
}

Map::~Map() {
	delete m_boundary;
}

const GeometryFactory::Ptr& Map::getGlobalFactory() const {
	return m_globalFactory;
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

