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
 * RandomWalkDriftScenario.h
 *
 *  Created on: May 5, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PARSERS_RANDOMWALKDRIFTSCENARIO_H_
#define INCLUDE_PARSERS_RANDOMWALKDRIFTSCENARIO_H_

#include <Distribution.h>

/**
 * This class encapsulates the parameters of the random walk with drift scenario. These parameters are:
 * \li the probability distribution for the drift direction for the first half of the simulation time, trend angle 1;
 * \li the probability distribution for the the drift direction for the second half of the simulation time, trend angle 2;
 * \li the probability distribution for the direction angle in case the new position of the person is outside the map.
 * In this case the position outside the map is ignored and anew direction angle is generated hoping that the new position will
 * be inside the map.
 * For each person a "trend angle 1" and "trend angle 2" are generated at every time step of the simulation.
 */
class RandomWalkDriftScenario {
public:

	/**
	 * Default constructor.
	 */
	RandomWalkDriftScenario();

	/**
	 * Default destructor.
	 */
	virtual ~RandomWalkDriftScenario();

	/**
	 * Returns the probability distribution for the trend angle during the first half of the simulation.
	 * @return the probability distribution for the trend angle during the first half of the simulation.
	 */
	Distribution* getTrendAngle1Distribution() const;

	/**
	 * Sets the probability distribution for the trend angle during the first half of the simulation.
	 * @param trendAngle1 the probability distribution for the trend angle during the first half of the simulation.
	 */
	void setTrendAngle1Distribution(Distribution* trendAngle1);

	/**
	 * Returns the probability distribution for the trend angle during the second half of the simulation.
	 * @return the probability distribution for the trend angle during the second half of the simulation.
	 */
	Distribution* getTrendAngle2Distribution() const;

	/**
	 * Sets the probability distribution for the trend angle during the second half of the simulation.
	 * @param trendAngle2 the probability distribution for the trend angle during the second half of the simulation.
	 */
	void setTrendAngle2Distribution(Distribution* trendAngle2);

	/**
	 * Returns the probability distribution of the "return angle". This is the direction angle used when a new step would fall outside the map.
	 * Instead of taking this step, a new direction angle is used hoping that a step in this direction will fall inside the map.This process
	 * is repeated until the new position is inside the map.
	 * @return he probability distribution of the "return angle". This is the direction angle used when a new step would fall outside the map.
	 */
	Distribution* getReturnAngleDistribution() const;

	/**
	 * Sets the probability distribution of the "return angle". This is the direction angle used when a new step would fall outside the map.
	 * Instead of taking this step, a new direction angle is used hoping that a step in this direction will fall inside the map.This process
	 * is repeted until the new position is inside the map.
	 * @param returnAngleDistribution the probability distribution of the "return angle". This is the direction angle used when a new step would fall outside the map.
	 */
	void setReturnAngleDistribution(Distribution *returnAngleDistribution);

private:
	Distribution* m_trendAngle1Distribution;
	Distribution* m_trendAngle2Distribution;
	Distribution* m_returnAngleDistribution;
};

#endif /* INCLUDE_PARSERS_RANDOMWALKDRIFTSCENARIO_H_ */
