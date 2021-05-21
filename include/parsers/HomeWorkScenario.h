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
 * HomeWorkScenario.h
 *
 * Created on: Mar 11, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */


#ifndef INCLUDE_HOMEWORKSCENARIO_H_
#define INCLUDE_HOMEWORKSCENARIO_H_



#include <Distribution.h>
#include <geos/geom/Point.h>
#include <parsers/HomeWorkLocation.h>
#include <vector>

using namespace std;
using namespace geos::geom;

/**
 * This class contains the parameters of an home-work scenario. Such a scenario has
 * a set of "home locations", "work locations" and "anchor point locations" kept internally
 * as HomeWorkLocation objects in 3 vectors. It also keeps the percent from the total simulation time
 * spent at home, at work and at an anchor point and spent during the trip from home to work. The speed
 * specified in the persons configuration file is ignored by the Person objects traveling from between home, work,
 * and anchor point locations and it is computed inside HomeWorkDisplacement object as the  distance between home and work locations
 * divided by the time duration allocated to this trip (total simulation time times spent during the trip from home to work).
 * Not every persons in a simulation will follow this type of displacement (home - work - anchor point). A percent
 * of the total number of persons specified in the simulation configuration file will follow a random walk behavior.
 * To generate the home location of a person, first a  HomeWorkLocation object is selected randomly from the vector storing these objects.
 * Then, a normally distributed location with the mean and standard deviation taken from  HomeWorkLocation object is generated.
 * The work location is generated similarly, selecting a HomeWorkLocation object from the vector storing these objects and then
 * an uniformly distributed point is generated inside the ellipse with the center and radii taken from HomeWorkLocation object.
 * Some persons are selected to stop at an anchor point during their trip from work to home. This selection is made according to a
 * Bernoulli distribution with the parameter read from the simulation configuration file. The anchor point location on map
 * is generated similar to the work locations. After the person stops at the anchor point he/she will go toward the home location.
 *  The road from two locations on the map 9home, work, anchor point) is generated as follows. At each time instant, the direction angle
 *  from the current location to the destination location is computed. Then a small random value is generated according to the "angle distribution"
 *  specified in the simulation configuration file. This value is added to the direction angle and then the person makes a step in this
 *  direction. By adding this random value the road from source location to destination will have a jagged shape.
 */
class HomeWorkScenario {
public:

	/**
	 * Default constructor of the class. It initializes the members of this class with default values (all percents are set to -1, the probability
	 * to stop at an anchor point to 0 and the angle distribution with nullptr.
	 */
	HomeWorkScenario();

	/**
	 * Default destructor.
	 */
	virtual ~HomeWorkScenario();


	void addHomeLocation(HomeWorkLocation h);

	void addWorkLocation(HomeWorkLocation h);

	void addAnchorLocation(HomeWorkLocation h);

	vector<HomeWorkLocation> getHomeLocations() const;

	vector<HomeWorkLocation> getWorkLocations() const;

	vector<HomeWorkLocation> getAnchorLocations() const;

	double getPrecentTimeHome() const;

	void setPrecentTimeHome(double precentTimeHome);

	double getPrecentTimeWork() const;

	void setPrecentTimeWork(double precentTimeWork);

	double getPrecentTimeTravel() const;

	void setPrecentTimeTravel(double precentTimetravel);

	double getPrecentTimeAnchorPoint() const;

	void setPrecentTimeAnchorPoint(double precentTimeAnchor);

	double getProbAnchorPoint() const;

	void setProbAnchorPoint(double precentTimeAnchor);

	const string toString();

	void setAngleDistribution(Distribution* distr);

	Distribution* getAngleDistribution();

private:
	vector<HomeWorkLocation> m_homeLocations;
	vector<HomeWorkLocation> m_workLocations;
	vector<HomeWorkLocation> m_anchorLocations;
	double m_precentTimeHome;
	double m_precentTimeWork;
	double m_precentTimeAnchor;
	double m_precentTimeTravel;
	double m_probAnchorPoint;
	Distribution* m_angleDistribution;
};

#endif /* INCLUDE_HOMEWORKSCENARIO_H_ */
