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
 * MahattanScenario.cpp
 *
 *  Created on: May 20, 2021
 *      Author: Bogdan Oancea
 *      Email:  bogdan.oancea@gmail.com
 */

#include <parsers/ManhattanScenario.h>
#include <Constants.h>
#include <sstream>

ManhattanScenario::ManhattanScenario() {
	// TODO Auto-generated constructor stub
}

ManhattanScenario::~ManhattanScenario() {
	// TODO Auto-generated destructor stub
}

double ManhattanScenario::getXStep() const {
	return m_XStep;
}

void ManhattanScenario::setXStep(double xs) {
	m_XStep = xs;
}

double ManhattanScenario::getYStep() const {
	return m_YStep;
}

void ManhattanScenario::setYStep(double ys) {
	m_YStep = ys;
}

double ManhattanScenario::getXOrigin() const {
	return m_XOrigin;
}

void ManhattanScenario::setXOrigin(double xOrigin) {
	m_XOrigin = xOrigin;
}

double ManhattanScenario::getYOrigin() const {
	return m_YOrigin;
}

void ManhattanScenario::setYOrigin(double yOrigin) {
	m_YOrigin = yOrigin;
}

const string ManhattanScenario::toString() const {
	ostringstream result;
	result << left << fixed << "Origin X: " << m_XOrigin << " Origin Y: " << m_YOrigin << " X Step: " << m_XStep << " Y Step: " << m_YStep <<endl;
	return result.str();
}

