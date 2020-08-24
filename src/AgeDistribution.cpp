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
 * AgeDistribution.cpp
 *
 *  Created on: Aug 14, 2020
 *      Author: Bogdan.Oancea
 */

#include <AgeDistribution.h>

AgeDistribution::AgeDistribution(int min_age, int max_age): m_minAge(min_age), m_maxAge(max_age) {
}

AgeDistribution::~AgeDistribution() {
	// TODO Auto-generated destructor stub
}

int AgeDistribution::getMaxAge() const {
	return m_maxAge;
}

void AgeDistribution::setMaxAge(int maxAge) {
	m_maxAge = maxAge;
}

int AgeDistribution::getMinAge() const {
	return m_minAge;
}

void AgeDistribution::setMinAge(int minAge) {
	m_minAge = minAge;
}
