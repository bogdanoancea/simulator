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
 * AgeDistribution.h
 *
 *  Created on: Aug 14, 2020
 *      Author: Bogdan.Oancea
 */

#ifndef INCLUDE_AGEDISTRIBUTION_H_
#define INCLUDE_AGEDISTRIBUTION_H_

class AgeDistribution {
public:
	AgeDistribution(int min_age, int max_age);
	virtual ~AgeDistribution();
	int getMaxAge() const;
	void setMaxAge(int maxAge);
	int getMinAge() const;
	void setMinAge(int minAge);

private:
	int m_minAge;
	int m_maxAge;
};

#endif /* INCLUDE_AGEDISTRIBUTION_H_ */
