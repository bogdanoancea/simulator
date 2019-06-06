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
#include <algorithm>

using namespace std;
using namespace geos;
using namespace geos::geom;

Grid::Grid(Map* map, double xOrig, double yOrig, double xTiledim,
		double yTiledim, unsigned long noTilesX, unsigned long noTilesY) :
		m_map { map }, m_xOrigin { xOrig }, m_yOrigin { yOrig }, m_xTileDim {
				xTiledim }, m_yTileDim { yTiledim }, m_noTilesX { noTilesX }, m_noTilesY { noTilesY}
				{
	m_tileCenters = computeTileCenters();
}

Grid::~Grid() {
	delete[] m_tileCenters;
}

string Grid::toString() const {
	ostringstream ss;

	ss << left << "Origin X" << Constants::sep << "Origin Y" << Constants::sep
			<< "X Tile Dim" << Constants::sep << "Y Tile Dim" << Constants::sep
			<< "No Tiles X" << Constants::sep << "No Tiles Y" << endl;

	ss << left << m_xOrigin << Constants::sep << m_yOrigin << Constants::sep
			<< m_xTileDim << Constants::sep << m_yTileDim << Constants::sep
			<< m_noTilesX << Constants::sep << m_noTilesY << endl;
	return (ss.str());
}

unsigned long Grid::getNoTiles() const {
	return (m_noTilesX * m_noTilesY);
}

vector<double> Grid::computeProbability(unsigned long t, /*unsigned long tileIndex,*/
MobilePhone* m, vector<AntennaInfo>& data,
		std::pair<um_iterator, um_iterator> antennas_iterator) {

	vector<double> result;
	vector<double> sumQuality = EMField::instance()->sumSignalQuality(this);
	// take the mobile phone and see which is the antenna connected to
	vector<AntennaInfo>::iterator ai;
	bool found = false;
	for (vector<AntennaInfo>::iterator i = data.begin(); i != data.end(); i++) {
		ai = i;
		if (ai->getTime() == t && ai->getDeviceId() == m->getId() && (ai->getEventCode() == static_cast<int>(EventType::ATTACH_DEVICE)
						|| ai->getEventCode() == static_cast<int>(EventType::ALREADY_ATTACHED_DEVICE))) {
			found = true;
			break;
		}
	}

	double sum = 0.0;
	for (unsigned long tileIndex = 0; tileIndex < getNoTiles(); tileIndex++) {
		if (found) {
			Coordinate c = getTileCenter(tileIndex);
			unsigned long antennaId = ai->getAntennaId();
			Antenna* a = nullptr;
			for (auto it = antennas_iterator.first;	it != antennas_iterator.second; it++) {
				a = dynamic_cast<Antenna*>(it->second);
				if (a->getId() == antennaId)
					break;
			}
			Point* p = m_map->getGlobalFactory()->createPoint(c);
			double lh = 1.0;
			if (a != nullptr) {
				lh = a->computeSignalQuality(p);
				//lh = EMField::instance()->connectionLikelihood(a, p);
				sum += lh;
			}
			cout << " time " << ai->getTime() << " tileIndex " << tileIndex << " tile center " << getTileCenter(tileIndex) << " connection likelihood " << lh << " antenna id " << a->getId() <<endl;
			result.push_back( lh);
			m_map->getGlobalFactory()->destroyGeometry(p);
		} else
			result.push_back(0.0);
	}

	for( auto& i: result )
	    //i /= (this->m_noTilesX * this->m_noTilesY);
		i /= sum;

	return (result);
}


Coordinate Grid::getTileCenter(unsigned long tileIndex) {
	return (m_tileCenters[tileIndex]);
}

Coordinate* Grid::computeTileCenters() {

	Coordinate* tileCenters = new Coordinate[getNoTiles()];

	for (unsigned long tileIndex = 0; tileIndex < getNoTiles(); tileIndex++) {
		Coordinate result;
		unsigned long nrow = tileIndex / m_noTilesX;
		unsigned long ncol = tileIndex - nrow * m_noTilesX;
		double x = ncol * m_xTileDim + m_xTileDim / 2.0;
		double y = nrow * m_yTileDim + m_yTileDim / 2.0;
		result.x = x;
		result.y = y;
		tileCenters[tileIndex] = result;
	}
	return tileCenters;
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
	throw runtime_error("Not yet implemented");
	return (result);
}

unsigned long Grid::getTileIndexY(Point* p) {
	unsigned long result = -1;
	throw runtime_error("Not yet implemented");
	return (result);
}

unsigned long Grid::getTileCenterX(Point* p) {
	unsigned long result = -1;
	throw runtime_error("Not yet implemented");
	return (result);
}

unsigned long Grid::getTileCenterY(Point* p) {
	unsigned long result = -1;
	throw runtime_error("Not yet implemented");
	return (result);
}
