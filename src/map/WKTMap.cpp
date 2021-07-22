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
 * WKTMap.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <geos/geom/Geometry.h>
#include <geos/geom/PrecisionModel.h>
#include <geos/io/WKTReader.h>
#include <geos/version.h>
#include <map/WKTMap.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::io;

WKTMap::WKTMap(string wktFileName) :
		Map(wktFileName) {
	const PrecisionModel* pm = new PrecisionModel(2.0, 0.0, 0.0);
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
#if GEOS_VERSION_MAJOR >= 3
	#if GEOS_VERSION_MINOR > 7
		m_boundary = reader.read(buffer.str()).release();
	#else
		m_boundary = reader.read(buffer.str());
	#endif
#else
		throw std::runtime_error("unsupported geos version");
#endif

	try {
		wktFile.close();
	} catch (ofstream::failure& e) {
		cerr << "Error closing map file!" << endl;
		throw e;
	}
	m_grid = nullptr;
}

WKTMap::~WKTMap() {
	if (m_boundary != nullptr)
		m_globalFactory->destroyGeometry(m_boundary);
}

const GeometryFactory::Ptr& WKTMap::getGlobalFactory() const {
	return (m_globalFactory);
}

Geometry* WKTMap::getBoundary() const {
	return (m_boundary);
}

Geometry* WKTMap::getEnvelope() const {
#if GEOS_VERSION_MAJOR >= 3
#if GEOS_VERSION_MINOR > 7
		return (m_boundary->getEnvelope().release());
#else
		return (m_boundary->getEnvelope());
#endif
#else
		throw std::runtime_error("unsupported geos version");
#endif
}
