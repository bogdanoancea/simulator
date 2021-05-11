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


class HomeWorkScenario {
public:
	HomeWorkScenario();
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
