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
 * Antenna.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <agent/Antenna.h>
#include <Clock.h>
#include <Constants.h>
#include <EMField.h>
#include <geos/geom/Coordinate.inl>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Point.h>
#include <geos/geom/Polygon.h>
#include <geos/io/WKTWriter.h>
#include <geos/util/GeometricShapeFactory.h>
#include <map/Map.h>
#include <RandomNumberGenerator.h>
#include <TinyXML2.h>
#include <Utils.h>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <memory>
#include <sstream>

using namespace tinyxml2;
using namespace std;
using namespace utils;

Antenna::Antenna(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double attenuationFactor, double power,
		unsigned long maxConnections, double smid, double ssteep, AntennaType type) :
		ImmovableAgent(m, id, initPosition, clock), m_ple { attenuationFactor }, m_power { power }, m_maxConnections { maxConnections }, m_Smid {
				smid }, m_SSteep { ssteep }, m_type { type }, m_height { Constants::ANTENNA_HEIGHT }, m_tilt { Constants::ANTENNA_TILT } {

	string fileName = getAntennaOutputFileName();
	char sep = Constants::sep;
	try {
		m_file.open(fileName, ios::out);
	}
	catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
	}
	m_file << "t" << sep << "AntennaId" << sep << "EventCode" << sep << "PhoneId" << sep << "x" << sep << "y" << sep << "TileId" << endl;
	m_S0 = 30 + 10 * log10(m_power);
	if (type == AntennaType::DIRECTIONAL) {
		m_beam_V = Constants::ANTENNA_BEAM_V;
		m_beam_H = Constants::ANTENNA_BEAM_H;
		m_azim_dB_Back = Constants::ANTENNA_AZIM_DB_BACK;
		m_elev_dB_Back = Constants::ANTENNA_ELEV_DB_BACK;
		m_direction = Constants::ANTENNA_DIRECTION;
	}
	setLocationWithElevation();
	m_cell = this->getMap()->getGlobalFactory()->createEmptyGeometry();
}

Antenna::Antenna(const Map* m, const Clock* clk, const unsigned long id, XMLElement* antennaEl, vector<MobileOperator*> mnos) :
		ImmovableAgent(m, id, nullptr, clk), m_cell { nullptr }, m_rmax { 0 }, m_handoverMechanism { HoldableAgent::CONNECTION_TYPE::UNKNOWN } {

	char sep = Constants::sep;
	XMLNode* n = getNode(antennaEl, "mno_name");
	if (n) {
		const char* mno_name = n->ToText()->Value();
		m_MNO = nullptr;
		for (unsigned int i = 0; i < mnos.size(); i++) {
			if (mnos.at(i)->getMNOName().compare(mno_name) == 0) {
				m_MNO = mnos.at(i);
				break;
			}
		}
		if (m_MNO == nullptr)
			throw runtime_error("Unknown MNO " + string(mno_name));
	}
	else {
		m_MNO = mnos.at(0);
	}

	m_maxConnections = getValue(antennaEl, "maxconnections", Constants::ANTENNA_MAX_CONNECTIONS);
	m_power = getValue(antennaEl, "power", Constants::ANTENNA_POWER);
	m_ple = getValue(antennaEl, "attenuationfactor", Constants::ATT_FACTOR);
	const char* t = getValue(antennaEl, "type", "omnidirectional");
	m_type = AntennaType::OMNIDIRECTIONAL;
	if (!strcmp(t, "directional"))
		m_type = AntennaType::DIRECTIONAL;

	m_Smin = getValue(antennaEl, "Smin", Constants::ANTENNA_SMIN);
	m_Qmin = getValue(antennaEl, "Qmin", Constants::ANTENNA_QMIN);
	m_Smid = getValue(antennaEl, "Smid", Constants::ANTENNA_S_MID);
	m_SSteep = getValue(antennaEl, "SSteep", Constants::ANTENNA_S_STEEP);
	double x = getValue(antennaEl, "x");
	double y = getValue(antennaEl, "y");
	m_height = getValue(antennaEl, "z", Constants::ANTENNA_HEIGHT);

//TODO get elevation from Grid
	Coordinate c = Coordinate(x, y, m_height);
	Point* p = getMap()->getGlobalFactory()->createPoint(c);
	setLocation(p);

	m_tilt = getValue(antennaEl, "tilt", Constants::ANTENNA_TILT);
	if (m_type == AntennaType::DIRECTIONAL) {
		m_azim_dB_Back = getValue(antennaEl, "azim_dB_back", Constants::ANTENNA_AZIM_DB_BACK);
		m_elev_dB_Back = getValue(antennaEl, "elev_dB_back", Constants::ANTENNA_ELEV_DB_BACK);
		m_beam_H = getValue(antennaEl, "beam_h", Constants::ANTENNA_BEAM_H);
		m_beam_V = getValue(antennaEl, "beam_v", Constants::ANTENNA_BEAM_V);
		m_direction = getValue(antennaEl, "direction", Constants::ANTENNA_DIRECTION);

		m_mapping_azim = createMapping(m_azim_dB_Back);
		m_mapping_elev = createMapping(m_elev_dB_Back);
		m_sd_azim = findSD(m_beam_H, m_azim_dB_Back, m_mapping_azim);
		m_sd_elev = findSD(m_beam_V, m_elev_dB_Back, m_mapping_elev);
	}
	string fileName = getAntennaOutputFileName();
	try {
		m_file.open(fileName, ios::out);
	}
	catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
	}
	m_file << "t" << sep << "AntennaId" << sep << "EventCode" << sep << "PhoneId" << sep << "x" << sep << "y" << sep << "TileId" << endl;
	m_S0 = 30 + 10 * log10(m_power);
}

Antenna::~Antenna() {
	if (m_file.is_open()) {
		try {
			m_file.close();
		}
		catch (std::ofstream::failure& e) {
			cerr << "Error closing output files!" << endl;
		}
	}
	if (m_cell) {
		this->getMap()->getGlobalFactory()->destroyGeometry(m_cell);
	}
}

const string Antenna::toString() const {
	ostringstream result;
	result << ImmovableAgent::toString() << left << setw(15) << m_power << setw(25) << m_maxConnections << setw(15) << m_ple << setw(15)
			<< m_MNO->getId();
	return (result.str());
}

bool Antenna::tryRegisterDevice(HoldableAgent* device) {
	bool result = false;
	if (getNumActiveConections() < m_maxConnections) {
		//if the device is not yet registered
		if (!alreadyRegistered(device)) {
			attachDevice(device);
			result = true;
		}
		else {
			registerEvent(device, EventType::ALREADY_ATTACHED_DEVICE, false);
			result = true;
		}
	}
	else {
		registerEvent(device, EventType::IN_RANGE_NOT_ATTACHED_DEVICE, false);
	}

	return (result);
}

void Antenna::attachDevice(HoldableAgent* device) {
	m_devices.push_back(device);
	registerEvent(device, EventType::ATTACH_DEVICE, false);
}

void Antenna::dettachDevice(HoldableAgent* device) {
	vector<HoldableAgent*>::iterator it = std::find(m_devices.begin(), m_devices.end(), device);
	if (it != m_devices.end()) {
		m_devices.erase(it);
	}
	registerEvent(device, EventType::DETACH_DEVICE, false);
}

bool Antenna::alreadyRegistered(HoldableAgent * device) {
	vector<HoldableAgent*>::iterator it = std::find(m_devices.begin(), m_devices.end(), device);
	if (it != m_devices.end())
		return (true);
	else
		return (false);
}

AntennaType Antenna::getType() const {
	return (m_type);
}

unsigned long Antenna::getNumActiveConections() {
	return (m_devices.size());
}

void Antenna::registerEvent(HoldableAgent * ag, const EventType event, const bool verbose) {
	char sep = Constants::sep;
	Point* loc = ag->getLocation();
	if (verbose) {
		cout << " Time: " << getClock()->getCurrentTime() << sep << " Antenna id: " << getId() << sep << " Event registered for device: "
				<< ag->getId() << sep;
		switch (event) {
			case EventType::ATTACH_DEVICE:
				cout << " Attached ";
				break;
			case EventType::DETACH_DEVICE:
				cout << " Detached ";
				break;
			case EventType::ALREADY_ATTACHED_DEVICE:
				cout << " In range, already attached ";
				break;
			case EventType::IN_RANGE_NOT_ATTACHED_DEVICE:
				cout << " In range, not attached ";
		}

		cout << sep << " Location: " << loc->getX() << sep << loc->getY() << ag->getMap()->getTileNo(loc);
		cout << endl;
	}
	else {
		stringstream ss;
		if (getMap()->hasGrid())
			ss << getClock()->getCurrentTime() << sep << getId() << sep << static_cast<int>(event) << sep << ag->getId() << sep << fixed
					<< loc->getX() << sep << loc->getY() << sep << getMap()->getTileNo(loc) << endl;

		if (m_file.is_open()) {
			m_file << ss.str();
			m_file.flush();
		}
		else
			cout << ss.str();
	}
}

double Antenna::S0() const {
	return (m_S0);
}

double Antenna::SDist(double dist) const {
	if (dist > 0)
		return (10.0 * m_ple * log10(dist));
	else
		return (0.0);
}

double Antenna::S(double dist) const {
	return (S0() - SDist(dist));
}

double Antenna::computeSignalQuality(const Point* p) const {
	double result = 0.0;
	const Coordinate* c = p->getCoordinate();
	result = computeSignalQuality(*c);
	return (result);
}

double Antenna::computeSignalQuality(const Coordinate c) const {
	double result = 0.0;
	if (m_type == AntennaType::OMNIDIRECTIONAL) {
		result = computeSignalQualityOmnidirectional(c);
	}
	if (m_type == AntennaType::DIRECTIONAL) {
		result = computeSignalQualityDirectional(c);
	}
	return (result);
}

double Antenna::computeSignalStrengthOmnidirectional(const Coordinate c) const {
	Point *p = getLocation();
	const double dz = c.z - p->getZ();
	const double dy = c.y - p->getY();
	const double dx = c.x - p->getX();

	const double dist = sqrt(dz * dz + dy * dy + dx * dx);
	return (S(dist));
}

double Antenna::computeSignalQualityOmnidirectional(const Coordinate c) const {
	double result = 0.0;
	double signalStrength = computeSignalStrengthOmnidirectional(c);
	result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
	return result;
}

double Antenna::computeSignalStrengthDirectional(const Coordinate c) const {
	double signalStrength = 0.0;
	Point *p = getLocation();
	const double dx = c.x - p->getX();
	const double dy = c.y - p->getY();
	const double dz = c.z - p->getZ();

	double dist = sqrt(dx * dx + dy * dy + dz * dz); //p->distance(getLocation());
	double distXY = sqrt(dx * dx + dy * dy);

	signalStrength += S(dist);
	double theta_azim = 90 - r2d(atan2(dy, dx));
	if (theta_azim < 0)
		theta_azim += 360;

	//cout << "theta_azim = " << theta_azim << endl;

	double azim = fmod(theta_azim - m_direction, 360);
	if (azim > 180)
		azim -= 360;
	if (azim < -180)
		azim += 360;

	//cout << "azim = " << azim << endl;

	//project azim to elevation plane -> azim2
	double r_azim = d2r(azim);
	//cout << "r_azim = " << r_azim << endl;
	double a = sin(r_azim) * distXY;
	double b = cos(r_azim) * distXY;
	//cout << "a=" << a << " b=" << b << endl;

	double e = projectToEPlane(b, m_height - c.z, m_tilt);
	double azim2 = r2d(atan2(a, e));
	signalStrength += norm_dBLoss(azim2, m_azim_dB_Back, m_sd_azim);

	//cout << "2:" << signalStrength << endl;
//vertical component
	double gamma_elevation = r2d(atan2(-dz, distXY));
	double elevation = (static_cast<int>(gamma_elevation - m_tilt)) % 360;
	if (elevation > 180)
		elevation -= 360;
	if (elevation < -180)
		elevation += 360;

	signalStrength += norm_dBLoss(elevation, m_elev_dB_Back, m_sd_elev);
	//cout << "3:" << signalStrength << endl;
	return signalStrength;
}

double Antenna::computeSignalQualityDirectional(const Coordinate c) const {
	double result;
	double signalStrength = computeSignalStrengthDirectional(c);
	result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
	return (result);
}

//TODO
double Antenna::findSD(double beamWidth, double dbBack, vector<pair<double, double>>& mapping) const {
	double result = 0.0;
	vector<double> tmp;
	double halfBeamWidth = beamWidth / 2.0;
	for (auto i : mapping) {
		tmp.push_back(fabs(i.first - halfBeamWidth));
	}
	int indexMin = std::min_element(tmp.begin(), tmp.end()) - tmp.begin();
	result = mapping[indexMin].second;
	//cout << "sd = " << result <<endl;
	return (result);
}

vector<pair<double, double>> Antenna::createMapping(double dbBack) const {
	vector<pair<double, double>> v;
	vector<pair<double, double>> result;
	double* sd = EMField::instance()->getSd();
	for (unsigned int i = 0; i < Constants::ANTENNA_MAPPING_N; i++) {
		double deg = getMin3db(sd[i], dbBack);
		pair<double, double> p = make_pair(sd[i], deg);
		v.push_back(p);
		//cout << " sd " << sd[i] << " deg " << deg << endl;
	}
	for (unsigned int i = 1; i <= 180; i++) {
		double sd = searchMin(i, v);
		pair<double, double> p = make_pair(i, sd);
		result.push_back(p);
	}
	return (result);
}

double Antenna::searchMin(double dg, vector<pair<double, double>> _3dBDegrees) const {
	for (pair<double, double>& i : _3dBDegrees) {
		i.second -= dg;
		i.second = fabs(i.second);
		//cout << "deg " << i.second << endl;
	}
	int minElementIndex = std::min_element(begin(_3dBDegrees), end(_3dBDegrees),
			[](const pair<double, double>& a, const pair<double, double>& b) {
				return a.second < b.second;
			}) - begin(_3dBDegrees);

//cout << "index min " << minElementIndex << endl;
	return (_3dBDegrees[minElementIndex].first);
}

double Antenna::getMin3db(double sd, double dbBack) const {
	vector<double> v;
	const double* p1 = EMField::instance()->getAntennaMin3DbArray();
	for (unsigned int i = 0; i < Constants::ANTENNA_MIN_3_DB; i++) {
		double p2 = fabs(-3.0 - norm_dBLoss(p1[i], dbBack, sd));
		v.push_back(p2);
	}
	int minElementIndex = std::min_element(v.begin(), v.end()) - v.begin();
	return (minElementIndex * 180.0 / (Constants::ANTENNA_MIN_3_DB - 1));
}

double Antenna::norm_dBLoss(double angle, double dbBack, double sd) const {
	double a = normalizeAngle(angle);
	RandomNumberGenerator* rand = RandomNumberGenerator::instance();
	double tmp = rand->normal_pdf(0.0, 0.0, sd);
	double inflate = -dbBack / (tmp - rand->normal_pdf(180.0, 0.0, sd));
	return ((rand->normal_pdf(a, 0.0, sd) - tmp) * inflate);
}

double Antenna::normalizeAngle(double angle) const {
	double a = fmod(angle, 360);
	if (a > 180)
		a = 360 - a;
	return (a);
}

double Antenna::projectToEPlane(double b, double c, double beta) const {
	double result;
	double d = sqrt(b * b + c * c);
	double lambda = r2d(atan2(c, fabs(b)));
	int cc;
	if (b > 0)
		if (beta < lambda)
			cc = 1;
		else
			cc = 2;
	else if (lambda + beta < 90)
		cc = 3;
	else
		cc = 4;

	switch (cc) {
		case 1:
			result = cos(d2r(lambda - beta)) * d;
			break;
		case 2:
			result = cos(d2r(beta - lambda)) * d;
			break;
		case 3:
			result = -cos(d2r(lambda + beta)) * d;
			break;
		case 4:
			result = cos(d2r(180 - lambda - beta)) * d;
			break;
	}
	return (result);
}

//TODO see if it is really needed
double Antenna::computePower(const Point* p) const {
	double result = 0.0;
	if (m_type == AntennaType::OMNIDIRECTIONAL)
		result = m_power * pow(p->distance(getLocation()), -m_ple);

	return (result);
}

double Antenna::computePower(const Coordinate c) const {
	double result = 0.0;
	const Coordinate dest = *(getLocation()->getCoordinate());
	if (m_type == AntennaType::OMNIDIRECTIONAL)
		result = m_power * pow(c.distance(dest), -m_ple);
	return (result);
}

void Antenna::setLocationWithElevation() {
	Point* p = getLocation();
	Point* newP = getMap()->getGlobalFactory()->createPoint(Coordinate(p->getX(), p->getY(), m_height));
	setLocation(newP);
	getMap()->getGlobalFactory()->destroyGeometry(p);
}

MobileOperator* Antenna::getMNO() const {
	return (m_MNO);
}

string Antenna::getAntennaOutputFileName() const {
	return (string("Antenna") + std::to_string(getId()) + "_MNO_" + m_MNO->getMNOName() + ".csv");
}

double Antenna::getRmax() const {
	return (m_rmax);
}

string Antenna::dumpCell() const {
	geos::io::WKTWriter writter;
	ostringstream result;
	result << writter.write(m_cell) << endl;
	return (result.str());
}

Geometry* Antenna::getCoverageArea() {
	if (m_type == AntennaType::OMNIDIRECTIONAL)
		return getCoverageAreaOmnidirectional();
	else if (m_type == AntennaType::DIRECTIONAL)
		return getCoverageAreaDirectional();
	else
		throw runtime_error("Coverage area: unknown antenna type!");
}

Geometry* Antenna::getCoverageAreaOmnidirectional() {
	double x = getLocation()->getX();
	double y = getLocation()->getY();
	geos::util::GeometricShapeFactory shapefactory(this->getMap()->getGlobalFactory().get());
	shapefactory.setCentre(Coordinate(x, y));
	shapefactory.setSize(2 * m_rmax);
	return (shapefactory.createCircle());
}

Geometry* Antenna::getCoverageAreaDirectional() {
	CoordinateSequence* cl = new CoordinateArraySequence();
	Coordinate init;
	double x = getLocation()->getX();
	double y = getLocation()->getY();
	const unsigned N = 100;
	for (double i = 0; i < N; i++) {
		double angle = i * (2 * utils::PI / 100);
		double dist = m_rmax;
		double delta_dist = 0.01 * dist;
		double xx = x + dist * sin(angle);
		double yy = y + dist * cos(angle);
		double S_actual = 0.0;

		if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
			S_actual = computeSignalStrengthDirectional(Coordinate(xx, yy, 0));
		else if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
			S_actual = computeSignalQualityDirectional(Coordinate(xx, yy, 0));

		unsigned k = 0;
		double min = 0.0;

		if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
			min = m_Smin;
		else if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
			min = m_Qmin;

		while (S_actual <= min && k < N) {
			dist -= delta_dist;
			xx = x + dist * sin(angle);
			yy = y + dist * cos(angle);

			if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
				S_actual = computeSignalStrengthDirectional(Coordinate(xx, yy, 0));
			else if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
				S_actual = computeSignalQualityDirectional(Coordinate(xx, yy, 0));
			k++;
		}
		if (k == N) {
			cout << "Warning: error computing coverage area for directional antenna: " << getId() << endl;
			cout << "Check the parameters of the antenna (Smin)!" << endl;
			cout << "The coverage area will be approximated with a circle regardless of the directional effect" << endl;
			delete cl;
			cl = nullptr;
			break;
		}
		if (angle == 0) {
			init = Coordinate(xx, yy);
			cl->add(init);
		}
		else
			cl->add(Coordinate(xx, yy));
	}
	if (cl != nullptr) {
		cl->add(init);
		LinearRing* lr = this->getMap()->getGlobalFactory()->createLinearRing(cl);
		return (this->getMap()->getGlobalFactory()->createPolygon(lr, nullptr));
	}
	else
		return (getCoverageAreaOmnidirectional());
}

double Antenna::computeSignalStrength(const Point* p) const {
	double result = 0.0;
	const Coordinate* c = p->getCoordinate();
	result = computeSignalStrength(*c);
	return (result);
}

double Antenna::computeSignalStrength(const Coordinate c) const {
	double result = 0.0;
	if (m_type == AntennaType::OMNIDIRECTIONAL) {
		result = computeSignalStrengthOmnidirectional(c);
	}
	if (m_type == AntennaType::DIRECTIONAL) {
		result = computeSignalStrengthDirectional(c);
	}
	return (result);
}

double Antenna::computeSignalMeasure(HoldableAgent::CONNECTION_TYPE handoverType, const Coordinate c) const {
	double result = 0.0;
	switch (handoverType) {
		case HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY:
			result = computeSignalQuality(c);
			break;
		case HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH:
			result = computeSignalStrength(c);
			break;
		case HoldableAgent::CONNECTION_TYPE::USING_POWER:
			result = computePower(c);
			break;
		case HoldableAgent::UNKNOWN:
			throw runtime_error("Unknown connection mechanism! Available values: power, quality, strength");
			break;
	}
	return result;
}


void Antenna::setHandoverMechanism(HoldableAgent::CONNECTION_TYPE handoverMechanism) {
	m_handoverMechanism = handoverMechanism;
	if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_STRENGTH)
		m_rmax = pow(10, (3 - m_Smin / 10) / m_ple) * pow(m_power, 1 / m_ple);
	else if (m_handoverMechanism == HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY)
		m_rmax = pow(10, (m_S0 - m_Smid + (1.0 / m_SSteep) * log(1.0 / m_Qmin - 1)) / (10 * m_ple));
	m_cell = getCoverageArea();
}

const string Antenna::getHeader() {
	ostringstream result;
	result << left << setw(15) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(15)
			<< "Max Connections" << setw(20) << "Attenuation Factor" << setw(15) << "MNO ID" << endl;
	return (result.str());
}

void Antenna::dumpSignal() const {
	char sep = Constants::sep;
	ofstream& qualityFile = getMNO()->getSignalFile();
	qualityFile << getId() << sep;
	unsigned long noTiles = getMap()->getNoTiles();
	Coordinate* tileCenters = getMap()->getTileCenters();
	for (unsigned long i = 0; i < noTiles - 1; i++) {
		qualityFile << computeSignalMeasure(m_handoverMechanism, tileCenters[i]) << sep;
	}
	qualityFile << computeSignalMeasure(m_handoverMechanism, tileCenters[noTiles - 1]) << endl;
}
