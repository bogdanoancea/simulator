/*
 * HomeWorkScenario.h
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_HOMEWORKSCENARIO_H_
#define INCLUDE_HOMEWORKSCENARIO_H_


#include <geos/geom/Point.h>
#include <parsers/HomeWorkLocation.h>
#include <parsers/Distribution.h>
#include <vector>

using namespace std;
using namespace geos::geom;


class HomeWorkScenario {
public:
	HomeWorkScenario();
	virtual ~HomeWorkScenario();


	void addHomeLocation(HomeWorkLocation h);
	void addWorkLocation(HomeWorkLocation h);
	vector<HomeWorkLocation> getHomeLocations() const;
	vector<HomeWorkLocation> getWorkLocations() const;
	double getPrecentTimeHome() const;
	void setPrecentTimeHome(double precentTimeHome);
	double getPrecentTimeWork() const;
	void setPrecentTimeWork(double precentTimeWork);
	const string toString();
	void setAngleDistribution(Distribution* distr);
	Distribution* getAngleDistribution();
private:
	vector<HomeWorkLocation> m_homeLocations;
	vector<HomeWorkLocation> m_workLocations;
	double m_precentTimeHome;
	double m_precentTimeWork;
	Distribution* m_angleDistribution;
};

#endif /* INCLUDE_HOMEWORKSCENARIO_H_ */
