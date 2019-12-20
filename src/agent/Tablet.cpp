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
 * Tablet.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/Tablet.h>
#include <iomanip>
#include <sstream>

Tablet::Tablet(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock) :
		HoldableAgent(m, id, initPosition, nullptr, clock) {
// TODO Auto-generated constructor stub

}

Tablet::~Tablet() {
	// TODO Auto-generated destructor stub
}

const string Tablet::toString() const {
	return (HoldableAgent::toString());
}

bool Tablet::tryConnect() {
	return false;
}


const string Tablet::getHeader()  {
	ostringstream result;
	result << left << setw(15) << "Tablet ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Speed " << setw(15) << " Owner id " << setw(15) << "MNO Id" << endl;
	return result.str();

}
