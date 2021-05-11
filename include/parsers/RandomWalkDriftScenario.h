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
