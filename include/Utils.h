/*
 * Data simulator for mobile phone network events
 *
 * utils.cpp
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 */

#ifndef UTILS_H
#define UTILS_H

#include <geos/geom/Point.h>
#include <TinyXML2.h>
#include <cmath>
#include <vector>

class Map;

using namespace geos;
using namespace geos::geom;
using namespace std;
using namespace tinyxml2;

/**
 * This namespace contains utility functions that don't belong to any class
 */
namespace utils {

	/**
	 * Generates n random points on a map
	 * @param m a pointer to the map where the points have to be located
	 * @param n the number of points to generate
	 * @return a vector of pointers to Point objects
	 */
	vector<Point*> generatePoints(Map* m, int n, unsigned seed);

	/**
	 * Prints out a header containing the names of the member variables from the Person class in a human readable format.
	 * It is used together with Person::toString() to output the Persons set on console
	 */
	void printPersonHeader();

	/**
	 * Prints out a header containing the names of the member variables from the Antenna class in a human readable format
	 * It is used together with Antenna::toString() to output the antennas set on console
	 */
	void printAntennaHeader();

	/**
	 * Prints out a header containing the names of the member variables from the MobilePhone class in a human readable format
	 * It is used together with MobilePhone::toString() to output the mobile phones set on console
	 */
	void printPhoneHeader();

	/**
	 * Prints out a header containing the names of the member variables from the MobileOperator class in a human readable format
	 * It is used together with MobileOperator::toString() to output the mobile network operators set on console
	 */
	void printMobileOperatorHeader();


	/**
	 * Number pi
	 */
	const double PI = std::atan(1.0) * 4;

	/**
	 * transforms a number from radians to degrees
	 * @param x the angle to be transformed from radians to degrees
	 * @return the value of x in degrees
	 */
	double r2d(double x);

	/**
	 * transforms a number from degrees to radians
	 * @param x the angle to be transformed from degrees to radians
	 * @return the value of x in radians
	 */
	double d2r(double x);
	/**
	 * Returns a pointer to an XMLNode with a specific name that belongs to an XMLElement. This function is used
	 * to parse the content of the configuration files.
	 * @param el the XMLElement where to search the XMLNode
	 * @param name the name of the node
	 * @return a pointer to an XMLNode with a specific name that belongs to an XMLElement
	 */
	XMLNode* getNode(XMLElement* el, const char* name);

	/**
	 * Retunrs a pointer to an XMLElement which is the first child of another XMLElement. This function is used
	 * to parse the content of the configuration files.
	 * @param el the parent XMLElement
	 * @param name the name of the child XMLElement
	 * @return a pointer to an XMLElement which is the first child of another XMLElement
	 */
	XMLElement* getFirstChildElement(XMLElement* el, const char* name) noexcept(false);

}

#endif
