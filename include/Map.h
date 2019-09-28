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

class Grid;

using namespace std;
using namespace geos;
using namespace geos::geom;

/**
 * This is the map where the simulation takes place. It could be a simple rectangle or any kind of geometry object(s)
 * read from a wkt file. The map has a boundary that is implemented as a Geometry object, a factory object of GeometryFactory type
 * used to create other objects. All geometric and geographic features of the simulator uses the GEOS library.
 * GEOS is an open source C++ library which is just a port to C++ of the well known Java Topology Suite.
 */

class Map {
public:
	/**
	 * Creates a map with a null boundary. The user may set the boundary later, using setBoundary() method.
	 */
	Map();

	/**
	 * Build a simple map of a rectangular shape.
	 * @param llX X coordinate of the bottom left corner of the rectangle.
	 * @param llY Y coordinate of the bottom left corner of the rectangle.
	 * @param width the width of the rectangle.
	 * @param height the height of the rectangle.
	 */
	Map(double llX, double llY, double width, double height);

	/**
	 * Builds a map reading it from a .wkt file.
	 * @param wktFile the name of the .wkt file that contains the description of the map.
	 * Currently, the first row of this file should contain the external boundary geometry of the map.
	 */
	Map(string wktFile);

	/**
	 * Default destructor
	 */
	virtual ~Map();

	/**
	 * Returns a pointer to the GeometryFactory object which is a factory object used to create other geometric objects. The GEOS
	 * library allows users to create geometric objects only using this factory, all the constructors are made private.
	 * @return a pointer to the GeometryFactory object used to create other geometric objects.
	 */
	const GeometryFactory::Ptr& getGlobalFactory() const;

	/**
	 * Returns a pointer to the Geometry object that represents the external boundary of the map.
	 * @return a pointer to the Geometry object that represents the external boundary of the map.
	 */
	Geometry* getBoundary() const;

	/**
	 * Sets the boundary of the Map object.
	 * @param boundary the boundary of the Map object.
	 */
	void setBoundary(Geometry* boundary);

	/**
	 * Returns a pointer to the Grid object associated with this Map. After creation of a Map the user should associate a Grid that overlaps
	 * this Map. The gird is used to compute the probability of the localization of different events during the simulation.
	 * @return a pointer the Grid object associated with this Map.
	 */
	const Grid* getGrid() const;

	/**
	 * Adds a Grid that overlaps this Map objects.
	 * @param dimTileX the dimension on OX of a tile of the Grid object.
	 * @param dimTileY the dimension on OY of a tile of the Grid object.
	 */
	void addGrid(double dimTileX, double dimTileY);


private:
	Polygon* create_rectangle(double llX, double llY, double width, double height);
	GeometryFactory::Ptr m_globalFactory;
	Geometry* m_boundary;
	Grid* m_grid;
};

#endif // MAP_H
