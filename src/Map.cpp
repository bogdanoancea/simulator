/*
 * Data simulator for mobile phone network events
 *
 * Map.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#include <geos/geom/PrecisionModel.h>
#include <Map.h>
#include <memory>

using namespace geos;
using namespace geos::geom;
using namespace std;

Map::Map() {

	// Define a precision model using 0,0 as the reference origin
	// and 2.0 as coordinates scale.
	PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
    // Initialize global factory with defined PrecisionModel
	// and a SRID of -1 (undefined).
	m_globalFactory = GeometryFactory::create(pm, -1);

    // We do not need PrecisionMode object anymore, it has
	// been copied to global_factory private storage
}

Map::~Map() {
	//dtor
}

const GeometryFactory::Ptr& Map::getGlobalFactory() const {
	return m_globalFactory;
}
