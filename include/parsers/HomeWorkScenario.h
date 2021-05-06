/*
 * HomeWorkScenario.h
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
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
