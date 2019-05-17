/*
 * Data simulator for mobile phone network events
 *
 * Grid.cpp
 *
 *  Created on: May 7, 2019
 *      Author: Bogdan Oancea
 */

#include <Grid.h>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <geos/geom/Coordinate.h>
#include <AgentsCollection.h>
#include <Agent.h>
#include <typeinfo>
#include <utility>
#include <unordered_map>
#include <EMField.h>

using namespace std;
using namespace geos;
using namespace geos::geom;

Grid::Grid(Map* map, double xOrig, double yOrig, double xTiledim, double yTiledim, unsigned long noTilesX, unsigned long noTilesY) :
		m_map { map }, m_xOrigin { xOrig }, m_yOrigin { yOrig }, m_xTileDim { xTiledim }, m_yTileDim { yTiledim }, m_noTilesX { noTilesX }, m_noTilesY {
				noTilesY } {
}

Grid::~Grid() {
	// TODO Auto-generated destructor stub
}

string Grid::toString() const {
	ostringstream ss;
	ss << left << setw(15) << "Origin X" << setw(15) << m_xOrigin << setw(15) << "Origin Y" << setw(15) << m_yOrigin << setw(15)
			<< "x tile dim" << setw(15) << m_xTileDim << setw(15) << "y tile dim" << setw(15) << m_yTileDim << setw(15) << "no tiles x"
			<< setw(15) << m_noTilesX << setw(15) << "no tiles y" << setw(15) << m_noTilesY << endl;
	return (ss.str());
}

unsigned long Grid::getNoTiles() const {
	return (m_noTilesX * m_noTilesY);
}

double Grid::computeProbability(unsigned long t, unsigned long tileIndex, MobilePhone* m, vector<AntennaInfo>& data,
		std::pair<um_iterator, um_iterator> itr) {
	double result = 0.0;
	// take the mobile phone and see which is the antenna connected to
	vector<AntennaInfo>::iterator ai;
	bool found = false;
	for (vector<AntennaInfo>::iterator i = data.begin(); i != data.end(); i++) {
		ai = i;
		if (ai->getTime() == t && ai->getDeviceId() == m->getId() && (ai->getEventCode() == 1 || ai->getEventCode() == 3)) {
			//std::cout << (*i).toString() << std::endl;
			found = true;
			break;
		}
	}
	if (found) {
		Coordinate c = getTileCenter(tileIndex);
		unsigned long antennaId = ai->getAntennaId();
		Antenna* a;
		for (auto it = itr.first; it != itr.second; it++) {
			a = dynamic_cast<Antenna*>(it->second);
			if (a->getId() == antennaId)
			break;
		}

		Point* p = m_map->getGlobalFactory()->createPoint(c);

		double lh = EMField::instance()->connectionLikelihood(a,p);
//		double qual = a->computeSignalQuality(p);
//		double sum_qual = 0.0;
//		for (auto it = itr.first; it != itr.second; it++) {
//			a = dynamic_cast<Antenna*>(it->second);
//			sum_qual += a->computeSignalQuality(p);
//		}
		//cout << p->getCoordinate()->x << "," << p->getCoordinate()->y << "," << qual << "," << sum_qual << endl;

		result = (1.0 / (m_noTilesX * m_noTilesY)) * lh;//qual / sum_qual;
	}
	else
		result = (1.0 / (m_noTilesX * m_noTilesY));
	return (result);
}

Coordinate Grid::getTileCenter(unsigned long tileIndex) {
	Coordinate result;
	unsigned long nrow = tileIndex / m_noTilesX;
	unsigned long ncol = tileIndex - nrow * m_noTilesX;
	double x = ncol * m_xTileDim + m_xTileDim / 2.0;
	double y = nrow * m_yTileDim + m_yTileDim / 2.0;
	result.x = x;
	result.y = y;
	return (result);
}

unsigned long Grid::getNoTilesX() const {
	return (m_noTilesX);
}

unsigned long Grid::getNoTilesY() const {
	return (m_noTilesY);
}

double Grid::getXTileDim() const {
	return (m_xTileDim);
}

double Grid::getXOrigin() const {
	return (m_xOrigin);
}

double Grid::getYTileDim() const {
	return (m_yTileDim);
}

double Grid::getYOrigin() const {
	return (m_yOrigin);
}

unsigned long Grid::getTileIndexX(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileIndexY(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileCenterX(Point* p) {
	unsigned long result = -1;

	return (result);
}

unsigned long Grid::getTileCenterY(Point* p) {
	unsigned long result = -1;

	return (result);
}
