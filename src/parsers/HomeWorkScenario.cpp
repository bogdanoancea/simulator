/*
 * HomeWorkScenario.cpp
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#include <parsers/HomeWorkScenario.h>

HomeWorkScenario::HomeWorkScenario() {
	// TODO Auto-generated constructor stub

}

HomeWorkScenario::~HomeWorkScenario() {
	// TODO Auto-generated destructor stub
}

void HomeWorkScenario::addHomeLocation(HomeWorkLocation h) {
	m_homeLocations.push_back(h);
}

void HomeWorkScenario::addWorkLocation(HomeWorkLocation w) {
	m_workLocations.push_back(w);
}

vector<HomeWorkLocation> HomeWorkScenario::getHomeLocations() const {
	return m_homeLocations;
}

vector<HomeWorkLocation> HomeWorkScenario::getWorkLocations() const {
	return m_workLocations;
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

double HomeWorkScenario::getRandomPopulation() const {
	return m_percentRandomPopulation;
}

void HomeWorkScenario::setRandomPopulation(double percentRandomPopulation) {
	m_percentRandomPopulation = percentRandomPopulation;
}
