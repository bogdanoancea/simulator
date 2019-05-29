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

/**
 * This is the map where the simulation takes place. It could be a simple rectangle or any kind of geometry object(s)
 * read from a wkt file. The map has a boundary that is implemented as a Geometry object, a factory object of GeometryFactory type
 * used to create other objects. All geometric and geographic features of the simulator uses the GEOS library.
 * GEOS is an open source C++ library which is just a port to C++ of the well know Java Topology Suite.
 */

class Map {
public:
	/**
	 * Creates a map with a null boundary. The user may set the boundary later, using setBoundary() method
	 */
	Map();

	/**
	 * Build a simple map of a rectangular shape
	 * @param llX X coordinate of the bottom left corner of the rectangle
	 * @param llY Y coordinate of the bottom left corner of the rectangle
	 * @param width the width of the rectangle
	 * @param height the height of the rectangle
	 */
	Map(double llX, double llY, double width, double height);

	/**
	 * Builds a map reading it from a .wkt file
	 * @param wktFile the name of the .wkt file that contains the description of the map.
	 */
	Map(string wktFile);

	/**
	 * Default destructor

	 */
	virtual ~Map();

	/**
	 * Returns a pointer to the GeometryFactory which is a factory object used to create other geometric objects. The GEOS
	 * library allows users to create geometric objects only using this factory, all the constructors are made private.
	 * @return a pointer to the GeometryFactory, a factory object used to create other geometric objects.
	 */
	const GeometryFactory::Ptr& getGlobalFactory() const;

	/**
	 * Returns a pointer to the Geometry object that represents the boundary of the map
	 * @return a pointer to the Geometry object that represents the boundary of the map
	 */
	Geometry* getBoundary() const {
		return m_boundary;
	}

	/**
	 * Sets the boundary of the map object
	 * @param boundary the boundary of the map object
	 */
	void setBoundary(Geometry* boundary) {
		m_boundary = boundary;
	}

private:
	Polygon* create_rectangle(double llX, double llY, double width, double height);

	GeometryFactory::Ptr m_globalFactory;

	Geometry* m_boundary;
};

#endif // MAP_H
