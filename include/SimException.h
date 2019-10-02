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
 * SimException.h
 * Created on: Aug 30, 2019
 *      Author: bogdanoancea
 */

#ifndef SIMEXCEPTION_H_
#define SIMEXCEPTION_H_

#include <exception>

using namespace std;

class SimException: public exception {

public:
	SimException(const char* msg);
	virtual const char * what () const throw();

	virtual ~SimException();

private:
	const char* msg;
};

#endif /* SIMEXCEPTION_H_ */
