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
 * utils.h
 *
 *  Created on: Apr 11, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef UTILS_H
#define UTILS_H

#include <agent/AgentsCollection.h>
#include <agent/MobilePhone.h>
#include <events/EventType.h>
#include <AntennaInfo.h>
#include <geos/geom/Point.h>
#include <PriorType.h>
#include <NetworkType.h>
#include <parsers/SimulationConfiguration.h>
#include <RandomNumberGenerator.h>
#include <cmath>
#include <utility>
#include <vector>
#include <TinyXML2.h>
#ifdef _MSC_VER
typedef __int32 int32_t;
#else
#include <stdint.h>
#endif


using namespace geos;
using namespace geos::geom;
using namespace std;
using namespace tinyxml2;

/**
 * This namespace contains utility functions that don't belong to any class.
 */
namespace utils {

	/**
	 * Generates n random points on a map.
	 * @param n the number of points to be generated.
	 * @param percentHome a percent of the total number of points considered to be "home locations", i.e. each time they
	 * have the same values. The rest of the points differ from one simulation to another.
	 * @return a vector of pointers to Point objects.
	 */
	vector<Point*> generatePoints(SimulationConfiguration* sc, unsigned long n, double percentHome);

	/**
	 * Generates a set of n random locations on a map. n * percentHome locations will be the same for every simulation,
	 * the rest will be different.
	 * @param sc a pointer to a SimulationConfiguration object need for get the Map where the locations should be generated.
	 * @param n the total number of locations to be generated.
	 * @param percentHome a value between 0 and 1 specifying the percent from the total number of points to be generated
	 * exactly the same for each simulation.
	 * @return a vector of n random locations on a map. n * percentHome locations will be the same for every simulation,
	 * the rest will be different.
	 */
	vector<Point*> generateRandomPoints(SimulationConfiguration* sc, unsigned long n, double percentHome);

	/**
	 * This function is used for the home-work scenario to generated the home location for each person. n*percentHome persons
	 * will have the initial location on the map in a "home location", the rest will be randomly distributed on the map.
	 * @param sc a pointer to a SimulationConfiguration object. This is need to extract the Map of the simulation, the seed to
	 * initialize the random number generator, and the home locations defined in the simulation configuration file.
	 * @param n the total number of locations to be generated.
	 * @param percentHome a value between 0 and 1 specifying the percent from the total number of person with a location in a "home location".
	 * @return a vector of locations on a map. A "home location" is generated according to a normal distribution with the parameters defined
	 * in a HomeLocation object read from the simulation configuration file.
	 */
	vector<Point*> generateHomeWorkPoints(SimulationConfiguration* sc, unsigned long n, double percentHome);

	/**
	 * Generates a number of npers "home locations".
	 * @param m the Map object of the simulation. All home locations will be inside this map.
	 * @param hLoc a HomeWorkLocation object containing the parameters of a home location. These parameters are read from the
	 * simulation configuration file.
	 * @param npers the total number of locations to be generated.
	 * @param random_gernerator a pointer to the RandomNumberGenerator object.
	 * @param result a vector with pointers to Point objects where the locations generated by this method are stored.
	 */
	void generateHomeLocation(Map* m, HomeWorkLocation hLoc, unsigned int npers, RandomNumberGenerator* random_gernerator, vector<Point*>& result);

	/**
	 * Generates n uniformly distributed locations on a map.
	 * @param m the map where the locations are to be generated.
	 * @param n the total number of locations to be generated.
	 * @param random_generator a pointer to the RandomNumberGenerator object.
	 * @return a vector with pointers to Point objects where the locations generated by this method are stored.
	 */
	vector<Point*> generateFixedPoints(const Map* m, unsigned long n, RandomNumberGenerator* random_generator);

	/**
	 * Number PI.
	 */
	const double PI = std::atan(1.0) * 4;

	/**
	 * Transforms a number from radians to degrees.
	 * @param x the angle to be transformed from radians to degrees.
	 * @return the value of x in degrees.
	 */
	inline double r2d(double x) {
		return x * (180.0 / PI);
	}

	/**
	 * Transforms a number from degrees to radians.
	 * @param x the angle to be transformed from degrees to radians.
	 * @return the value of x in radians.
	 */
	inline double d2r(double x){
		return (PI / 180.0) * x;
	}


	/**
	 * Returns a pointer to an XMLNode with a specific name that belongs to an XMLElement. This function is used
	 * to parse the content of the configuration files.
	 * @param el a pointer to the the XMLElement where to search the XMLNode.
	 * @param name the name of the node.
	 * @return a pointer to an XMLNode with a specific name that belongs to an XMLElement.
	 */
	XMLNode* getNode(XMLElement* el, const char* name);

	/**
	 * Returns a pointer to an XMLElement which is the first child of another XMLElement. This function is used
	 * to parse the content of the configuration files.
	 * @param el a pointer to the parent XMLElement.
	 * @param name the name of the child XMLElement.
	 * @return a pointer to an XMLElement which is the first child of another XMLElement.
	 */
	XMLElement* getFirstChildElement(XMLElement* el, const char* name) noexcept(false);


	/**
	 * Returns a double value obtained by converting the text in an XMLNode to a double. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn't find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return a double value obtained by converting the text in an XMLNode to a double.
	 */
	double 	getValue(XMLElement* el, const char* name, double default_value);


	/**
	 * Returns an int value obtained by converting the text in an XMLNode to an int. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn't find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return an int value obtained by converting the text in an XMLNode to an int.
	 */
	int getValue(XMLElement* el, const char* name, int default_value);

	/**
	 * Returns an unsigned long value obtained by converting the text in an XMLNode to an unsigned long. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn't find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return an unsigned long value obtained by converting the text in an XMLNode to an unsigned long.
	 */
	unsigned long getValue(XMLElement* el, const char* name, unsigned long default_value);

	/**
	 * Returns a string (const char*) value obtained by converting the text in an XMLNode to a const char* pointer. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn't find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return a const char* value obtained by converting the text in an XMLNode to a const char*.
	 */
	const char* getValue(XMLElement* el, const char* name, const char* default_value);

	/**
	 * Returns a double value obtained by converting the text in an XMLNode to a double. In case the node does not exist
	 * this method throws an exception.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @return a double value obtained by converting the text in an XMLNode to a double
	 */
	double getValue(XMLElement* el, const char* name);

	/**
	 * Returns the network type obtained by converting the text in an XMLNode to a NetworkType value. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value is the value returned in case the function doesn't find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return the network type obtained by converting the text in an XMLNode to a NetworkType value.
	 */
	NetworkType getValue(XMLElement* el, const char* name, NetworkType default_value);

	/**
	 * Returns the event type obtained by converting the text in an XMLNode to a EventType value. In case
	 * the node does not exists, this function returns a default value passed as a parameter.
	 * @param el a pointer to the XMLElement where the XMLNode is located.
	 * @param name the name of the XMLNode.
	 * @param default_value value is the value returned in case the function doesn't find any
	 * XMLNode with the specified name under the XMLElement.
	 * @return the event type obtained by converting the text in an XMLNode to a EventType value.
	 */
	EventType getValue(XMLElement* el, const char* name, EventType default_value);

	/**
	 * Computes the inverse of the normal CDF.
	 * @param p the value of a probability.
	 * @param mu the mean for the normal distribution.
	 * @param sigma the standard deviation of the normal distribution.
	 * @return the value of the inverse normal CDF.
	 */
	double inverseNormalCDF(const double p, const double mu, const double sigma);


	/**
	 * Converts a NetworkType value into a string.
	 * @param type a NetworkType value.
	 * @return a NetworkType value converted to a string.
	 */
	string toString(NetworkType type);

	/**
	 * Needed to compute fast sin and cos
	 * @param x the value to be truncated
	 * @return
	 */
	int32_t fast_round(double x);

	/**
	 * Fast sin computation.
	 * @param x the angel in radians
	 * @return the value of sin function
	 */
	double fast_sin(double x);

}

#endif
