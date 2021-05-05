/*
 * LevyFlightScenario.h
 *
 *  Created on: May 5, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PARSERS_LEVYFLIGHTSCENARIO_H_
#define INCLUDE_PARSERS_LEVYFLIGHTSCENARIO_H_

#include <Distribution.h>

class LevyFlightScenario {
public:
	LevyFlightScenario();
	virtual ~LevyFlightScenario();
	unsigned int getCutOffPoint() const;
	void setCutOffPoint(unsigned int cutOffPoint);
	Distribution* getSpeedDistribution() const;
	void setSpeedDistribution(Distribution *speedDistribution);

private:
	Distribution* m_speedDistribution;
	unsigned int m_cutOffPoint;
};

#endif /* INCLUDE_PARSERS_LEVYFLIGHTSCENARIO_H_ */
