/*
 * RandomWalkDriftScenario.h
 *
 *  Created on: May 5, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PARSERS_RANDOMWALKDRIFTSCENARIO_H_
#define INCLUDE_PARSERS_RANDOMWALKDRIFTSCENARIO_H_

#include <Distribution.h>

class RandomWalkDriftScenario {
public:
	RandomWalkDriftScenario();
	virtual ~RandomWalkDriftScenario();

	Distribution* getTrendAngle1Distribution() const;
	void setTrendAngle1Distribution(Distribution* trendAngle1);
	Distribution* getTrendAngle2Distribution() const;
	void setTrendAngle2Distribution(Distribution* trendAngle2);
	Distribution* getReturnAngleDistribution() const;
	void setReturnAngleDistribution(Distribution *returnAngleDistribution);

private:
	Distribution*  m_trendAngle1Distribution;
	Distribution*  m_trendAngle2Distribution;
	Distribution* m_returnAngleDistribution;
};

#endif /* INCLUDE_PARSERS_RANDOMWALKDRIFTSCENARIO_H_ */
