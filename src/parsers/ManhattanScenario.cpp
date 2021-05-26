/*
 * MahattanScenario.cpp
 *
 *  Created on: May 20, 2021
 *      Author: bogdan
 */

#include <parsers/ManhattanScenario.h>

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


