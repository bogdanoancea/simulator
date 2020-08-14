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
 * TruncatedNormalAgeDistribution.cpp
 *
 *  Created on: Aug 14, 2020
 *      Author: Bogdan.Oancea
 */

#include <TruncatedNormalAgeDistribution.h>

TruncatedNormalAgeDistribution::TruncatedNormalAgeDistribution(int minAge, int maxAge, double meanAge, double sd): AgeDistribution(minAge, maxAge) {
	m_mean = meanAge;
	m_sd = sd;
}

TruncatedNormalAgeDistribution::~TruncatedNormalAgeDistribution() {
	// TODO Auto-generated destructor stub
}

double TruncatedNormalAgeDistribution::getMean() const {
	return m_mean;
}

void TruncatedNormalAgeDistribution::setMean(double mean) {
	m_mean = mean;
}

double TruncatedNormalAgeDistribution::getSd() const {
	return m_sd;
}

void TruncatedNormalAgeDistribution::setSd(double sd) {
	m_sd = sd;
}
