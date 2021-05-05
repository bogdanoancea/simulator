/*
 * RandomWalkDriftScenario.cpp
 *
 *  Created on: May 5, 2021
 *      Author: bogdan
 */

#include <parsers/RandomWalkDriftScenario.h>

RandomWalkDriftScenario::RandomWalkDriftScenario() {
	// TODO Auto-generated constructor stub

}

RandomWalkDriftScenario::~RandomWalkDriftScenario() {
	// TODO Auto-generated destructor stub
}

Distribution* RandomWalkDriftScenario::getTrendAngle1Distribution() const {
	return m_trendAngle1Distribution;
}

void RandomWalkDriftScenario::setTrendAngle1Distribution(Distribution* trendAngle1) {
	m_trendAngle1Distribution = trendAngle1;
}

Distribution* RandomWalkDriftScenario::getTrendAngle2Distribution() const {
	return m_trendAngle2Distribution;
}

void RandomWalkDriftScenario::setTrendAngle2Distribution(Distribution* trendAngle2) {
	m_trendAngle2Distribution = trendAngle2;
}

Distribution* RandomWalkDriftScenario::getReturnAngleDistribution() const {
	return m_returnAngleDistribution;
}

void RandomWalkDriftScenario::setReturnAngleDistribution(Distribution *returnAngleDistribution) {
	m_returnAngleDistribution = returnAngleDistribution;
}
