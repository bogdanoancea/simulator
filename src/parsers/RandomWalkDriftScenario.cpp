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
 * RandomWalkDriftScenario.cpp
 *
 *  Created on: May 5, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
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
