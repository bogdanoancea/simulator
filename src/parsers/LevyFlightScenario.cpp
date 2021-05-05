/*
 * LevyFlightScenario.cpp
 *
 *  Created on: May 5, 2021
 *      Author: bogdan
 */

#include <parsers/LevyFlightScenario.h>

LevyFlightScenario::LevyFlightScenario() {
	// TODO Auto-generated constructor stub

}

LevyFlightScenario::~LevyFlightScenario() {
	// TODO Auto-generated destructor stub
}

unsigned int LevyFlightScenario::getCutOffPoint() const {
	return m_cutOffPoint;
}

void LevyFlightScenario::setCutOffPoint(unsigned int cutOffPoint) {
	m_cutOffPoint = cutOffPoint;
}

Distribution* LevyFlightScenario::getSpeedDistribution() const {
	return m_speedDistribution;
}

void LevyFlightScenario::setSpeedDistribution(Distribution *speedDistribution) {
	m_speedDistribution = speedDistribution;
}
