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
 * LevyFlightScenario.h
 *
 *  Created on: May 5, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PARSERS_LEVYFLIGHTSCENARIO_H_
#define INCLUDE_PARSERS_LEVYFLIGHTSCENARIO_H_

#include <Distribution.h>

/**
 * This class encapsulates all the parameters needed by the Levy flight mobility pattern. Under this pattern, at each time step a new speed for each person
 * is generated according to a Levy distribution with the mean specified in the simulation configuration file. When a random value generated using this
 * distribution is very large, it is set to a fixed threshold value. In the simulation configuration file there is a parameter named "cut off point" which
 * is an integer. If the actual value of the speed is greater than the mean value times the value of the cutoff point, the speed is set to
 * the mean value times the value of the cutoff point.
 */
class LevyFlightScenario {
public:
	/**
	 * Default constructor of the class.
	 */
	LevyFlightScenario();

	/**
	 * Default destructor.
	 */
	virtual ~LevyFlightScenario();

	/**
	 * Return a value that represents a maximum level used to limit the speed. If the actual speed of a person is
	 * greater than the mean value times the value of the cutoff point, the speed is set to
	 * the mean value times the value of the cutoff point. This value is read from the simulation configuration file.
	 * @return
	 */
	unsigned int getCutOffPoint() const;

	/**
	 * Sets a value that represents a maximum level used to limit the speed. If the actual speed of a person is
	 * greater than the mean value times the value of the cutoff point, the speed is set to
	 * the mean value times the value of the cutoff point. This value is read from the simulation configuration file.
	 * @param cutOffPoint a value that represents a maximum level used to limit the speed. If the actual speed of a person is
	 * greater than the mean value times the value of the cutoff point, the speed is set to
	 * the mean value times the value of the cutoff point.
	 */
	void setCutOffPoint(unsigned int cutOffPoint);

	/**
	 * Returns a pointer to a Distribution object representing the probability distribution (Levy) used to generate the speed of each person.
	 * @return a pointer to a Distribution object representing the probability distribution (Levy) used to generate the speed of each person.
	 */
	Distribution* getSpeedDistribution() const;

	/**
	 * Sets the Distribution object representing the probability distribution (Levy) used to generate the speed of each person.
	 * @param speedDistribution a pointer to a Distribution object representing the probability distribution (Levy) used to generate the speed of each person.
	 */
	void setSpeedDistribution(Distribution *speedDistribution);

private:
	Distribution* m_speedDistribution;
	unsigned int m_cutOffPoint;
};

#endif /* INCLUDE_PARSERS_LEVYFLIGHTSCENARIO_H_ */
