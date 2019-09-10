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
#include <cmath>

using namespace std;
using namespace geos;
using namespace geos::geom;

Grid::Grid(double xOrig, double yOrig, double xTiledim, double yTiledim, unsigned long noTilesX, unsigned long noTilesY) :
		m_xOrigin { xOrig }, m_yOrigin { yOrig }, m_xTileDim { xTiledim }, m_yTileDim { yTiledim }, m_noTilesX { noTilesX }, m_noTilesY { noTilesY } {
	m_tileCenters = computeTileCenters();
}

Grid::~Grid() {
	delete[] m_tileCenters;
}

string Grid::toString() const {
	ostringstream ss;

	ss << left << "Origin X" << Constants::sep << "Origin Y" << Constants::sep << "X Tile Dim" << Constants::sep << "Y Tile Dim" << Constants::sep << "No Tiles X" << Constants::sep
			<< "No Tiles Y" << endl;

	ss << left << fixed << m_xOrigin << Constants::sep << m_yOrigin << Constants::sep << m_xTileDim << Constants::sep << m_yTileDim << Constants::sep << m_noTilesX
			<< Constants::sep << m_noTilesY << endl;
	return (ss.str());
}

const unsigned long Grid::getNoTiles() const {
	return (m_noTilesX * m_noTilesY);
}

vector<double> Grid::computeProbability(unsigned long t, MobilePhone* m, vector<AntennaInfo>& data, pair<um_iterator, um_iterator> antennas_iterator, PriorType prior) const {
	vector<double> result;
	// take the mobile phone and see which is the antenna connected to
	vector<AntennaInfo>::iterator ai;
	bool found = false;
	for (vector<AntennaInfo>::iterator i = data.begin(); i != data.end(); i++) {
		ai = i;
		if (ai->getTime() == t && ai->getDeviceId() == m->getId()
				&& (ai->getEventCode() == static_cast<int>(EventType::ATTACH_DEVICE) || ai->getEventCode() == static_cast<int>(EventType::ALREADY_ATTACHED_DEVICE))) {
			found = true;
			break;
		}
	}
	if (prior == PriorType::NETWORK)
		result = useNetworkPrior(t, found, ai, antennas_iterator);
	else if (prior == PriorType::UNIFORM)
		result = useUniformPrior(t, found, ai, antennas_iterator);
	return (result);
}

vector<double> Grid::useNetworkPrior(unsigned long t, bool connected, vector<AntennaInfo>::iterator ai, pair<um_iterator, um_iterator> antennas_iterator) const {
	vector<double> result;
	double sum = 0.0;
	for (unsigned long tileIndex = 0; tileIndex < getNoTiles(); tileIndex++) {
		double lh = 0.0;
		if (connected) {
			Coordinate c = getTileCenter(tileIndex);
			unsigned long antennaId = ai->getAntennaId();
			Antenna* a = nullptr;
			for (auto it = antennas_iterator.first; it != antennas_iterator.second; it++) {
				a = dynamic_cast<Antenna*>(it->second);
				if (a->getId() == antennaId)
					break;
			}
			c.z = 0; //TODO tile elevation!
			if (a != nullptr) {
				lh = a->computeSignalQuality(c);
				sum += lh;
			}
		}
		result.push_back(lh);
	}
	for (auto& i : result) {
		if (sum != 0.0) {
			i /= sum;
		}
	}
	return result;
}

vector<double> Grid::useUniformPrior(unsigned long t, bool connected, vector<AntennaInfo>::iterator ai, pair<um_iterator, um_iterator> antennas_iterator) const {
	vector<double> result;

	for (unsigned long tileIndex = 0; tileIndex < getNoTiles(); tileIndex++) {
		double lh = 0.0;
		if (connected) {
			unsigned long antennaId = ai->getAntennaId();
			Antenna* a = nullptr;
			for (auto it = antennas_iterator.first; it != antennas_iterator.second; it++) {
				a = dynamic_cast<Antenna*>(it->second);
				if (a->getId() == antennaId)
					break;
			}
			if (a != nullptr) {
				lh = EMField::instance()->connectionLikelihoodGrid(a, this, tileIndex);
			}
		}
		result.push_back(lh);
	}
	for (auto& i : result) {
		if (i > 0.0)
			i /= (m_noTilesX * m_noTilesY);
		else
			i = 1.0 / (m_noTilesX * m_noTilesY);
	}
	return result;
}

Coordinate Grid::getTileCenter(unsigned long tileIndex) const {
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
	return (tileCenters);
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

unsigned long Grid::getTileIndexX(const Point* p) const {
	double x = p->getX();
	return (getTileIndexX(x));
}

unsigned long Grid::getTileIndexX(double x) const {
	long result = -1;
	if (x < m_xOrigin || x > m_xOrigin + m_xTileDim * m_noTilesX) {
		result = -1;
	} else if (fmod((x - m_xOrigin), m_xTileDim) == 0) {
		result = (x - m_xOrigin) / m_xTileDim - 1;
		if (result < 0)
			result = 0;
	} else {
		result = (x - m_xOrigin) / m_xTileDim;
	}
	return (result);
}

unsigned long Grid::getTileIndexY(const Point* p) const {
	double y = p->getY();
	return (getTileIndexY(y));
}

unsigned long Grid::getTileIndexY(double y) const {
	long result = -1;

	if (y < m_yOrigin || y > m_yOrigin + m_yTileDim * m_noTilesY)
		result = -1;
	else if (fmod((y - m_yOrigin), m_yTileDim) == 0) {
		result = (y - m_yOrigin) / m_yTileDim - 1;
		if (result < 0)
			result = 0;
	} else {
		result = (y - m_yOrigin) / m_yTileDim;
	}
	return (result);
}

unsigned long Grid::getTileNo(const Point* p) const {
	unsigned long x = getTileIndexX(p);
	unsigned long y = getTileIndexY(p);
	return y * m_noTilesX + x;
}

unsigned long Grid::getTileNo(double x, double y) const {
	unsigned long i = getTileIndexX(x);
	unsigned long j = getTileIndexY(y);
	return j * m_noTilesX + i;
}

void Grid::dumpGrid(const string& gridFileName) const {
	ofstream gridFile;
	try {
		gridFile.open(gridFileName, ios::out);
	} catch (ofstream::failure& e) {
		cerr << "Error opening grid output files!" << endl;
	}

	gridFile << toString();
	try {
		gridFile.close();
	} catch (const ofstream::failure& e) {
		cerr << "Error closing grid file!" << endl;
	}

}
