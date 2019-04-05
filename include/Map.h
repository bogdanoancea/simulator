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
using namespace geos;
using namespace geos::geom;



class Map {
	public:
		/** Default constructor */
		Map();

		/** Default destructor */
		virtual ~Map();
		const GeometryFactory::Ptr& getGlobalFactory() const;

	private:
		GeometryFactory::Ptr m_globalFactory;

};

#endif // MAP_H
