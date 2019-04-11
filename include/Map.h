/*
 * Data simulator for mobile phone network events
 *
 * Map.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef MAP_H
#define MAP_H

#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Polygon.h>

using namespace geos;
using namespace geos::geom;


enum class Geometries {
	RECTANGLE
};

enum class MAP_TYPE {
	GEOM, FROM_FILE
};

class Map {
	public:
		/** Default constructor */
		Map();

		/** Default destructor */
		virtual ~Map();


		const GeometryFactory::Ptr& getGlobalFactory() const;
		Polygon* create_rectangle(double llX, double llY, double width,
				double height);



	private:
		GeometryFactory::Ptr m_globalFactory;

};

#endif // MAP_H
