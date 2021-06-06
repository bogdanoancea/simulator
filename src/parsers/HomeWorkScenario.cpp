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
 * HomeWorkScenario.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <parsers/HomeWorkScenario.h>
#include <sstream>
#include <string>


HomeWorkScenario::HomeWorkScenario() {
	m_precentTimeHome = -1;
	m_precentTimeWork = -1;
	m_precentTimeAnchor = -1;
	m_precentTimeTravel = -1;
	m_probAnchorPoint = 0;
	m_angleDistribution = nullptr;
}

HomeWorkScenario::~HomeWorkScenario() {
	// TODO Auto-generated destructor stub
	if(m_angleDistribution)
		delete m_angleDistribution;
}

void HomeWorkScenario::addHomeLocation(HomeWorkLocation h) {
	m_homeLocations.push_back(h);
}

void HomeWorkScenario::addWorkLocation(HomeWorkLocation w) {
	m_workLocations.push_back(w);
}

void HomeWorkScenario::addAnchorLocation(HomeWorkLocation a) {
	m_anchorLocations.push_back(a);
}

vector<HomeWorkLocation> HomeWorkScenario::getHomeLocations() const {
	return m_homeLocations;
}

vector<HomeWorkLocation> HomeWorkScenario::getWorkLocations() const {
	return m_workLocations;
}

vector<HomeWorkLocation> HomeWorkScenario::getAnchorLocations() const {
	return m_anchorLocations;
}

double HomeWorkScenario::getPrecentTimeHome() const {
	return m_precentTimeHome;
}

void HomeWorkScenario::setPrecentTimeHome(double precentTimeHome) {
	m_precentTimeHome = precentTimeHome;
}

double HomeWorkScenario::getPrecentTimeWork() const {
	return m_precentTimeWork;
}

void HomeWorkScenario::setPrecentTimeWork(double precentTimeWork) {
	m_precentTimeWork = precentTimeWork;
}


double HomeWorkScenario::getPrecentTimeAnchorPoint() const {
	return m_precentTimeAnchor;
}

void HomeWorkScenario::setPrecentTimeAnchorPoint(double precentTimeAnchor) {
	m_precentTimeAnchor = precentTimeAnchor;
}


double HomeWorkScenario::getPrecentTimeTravel() const {
	return m_precentTimeTravel;
}

void HomeWorkScenario::setPrecentTimeTravel(double precentTimetravel) {
	m_precentTimeTravel = precentTimetravel;
}

void HomeWorkScenario::setProbAnchorPoint(double prob) {
	m_probAnchorPoint = prob;
}

double HomeWorkScenario::getProbAnchorPoint() const {
	return m_probAnchorPoint;
}

void HomeWorkScenario::setAngleDistribution(Distribution* distr) {
	m_angleDistribution = distr;
}

Distribution* HomeWorkScenario::getAngleDistribution() {
	return m_angleDistribution;
}

const string HomeWorkScenario::toString(){
	ostringstream result;
	//output home locations
	result << "Home locations" << endl;
	for(auto& l : m_homeLocations) {
		result <<  l.toString() << endl;
	}
	//output works locations
	result << "Work locations" << endl;
	for(auto & w : m_workLocations) {
		result << w.toString() << endl;
	}
	//output anchor locations
	result << "Anchor locations" << endl;
	for(auto & w : m_anchorLocations) {
		result << w.toString() << endl;
	}

	return result.str();
}
