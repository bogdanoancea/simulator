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

using namespace std;
using namespace geos;
using namespace geos::geom;




class Map {
	public:
		/** Default constructor */
		Map();

		Map(double llX, double llY, double width, double height);
		Map(string wktFile);

		/** Default destructor */
		virtual ~Map();


		const GeometryFactory::Ptr& getGlobalFactory() const;


		Geometry* getBoundary() const {
			return m_boundary;
		}


		void setBoundary(Geometry* boundary) {
			m_boundary = boundary;
		}

	private:
		Polygon* create_rectangle(double llX, double llY, double width,
				double height);


		GeometryFactory::Ptr m_globalFactory;

		Geometry* m_boundary;
};

#endif // MAP_H
