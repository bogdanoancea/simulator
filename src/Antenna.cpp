#include <Antenna.h>
#include <HoldableAgent.h>
#include <EventType.h>
#include <Constants.h>
#include <geos/geom/GeometryFactory.h>
#include <Map.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <string.h>
#include <TinyXML2.h>
#include <Utils.h>

using namespace tinyxml2;
using namespace std;
using namespace utils;

Antenna::Antenna(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double attenuationFactor, double power, unsigned long maxConnections, double smid,
		double ssteep, AntennaType type) :
		ImmovableAgent(m, id, initPosition, clock), m_ple { attenuationFactor }, m_power { power }, m_maxConnections { maxConnections }, m_Smid { smid }, m_SSteep { ssteep }, m_type {
				type } , m_height {Constants::ANTENNA_HEIGHT}, m_tilt {Constants::ANTENNA_TILT}{

	string fileName = getName() + std::to_string(id) + ".csv";
	try {
		m_file.open(fileName, ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		cerr << "Output goes to the console!" << endl;
	}
	m_S0 = 30 + 10 * log10(m_power);
	if(type == AntennaType::DIRECTIONAL) {
		m_beam_V = Constants::ANTENNA_BEAM_V;
		m_beam_H = Constants::ANTENNA_BEAM_H;
		m_azim_dB_Back = Constants::ANTENNA_AZIM_DB_BACK;
		m_elev_dB_Back = Constants::ANTENNA_ELEV_DB_BACK;
		m_direction = Constants::ANTENNA_DIRECTION;
	}
	setLocationWithElevation();
	m_cell = this->getMap()->getGlobalFactory()->createPolygon();
}

Antenna::Antenna(const Map* m, const Clock* clk, const unsigned long id, XMLElement* antennaEl) :
		ImmovableAgent(m, id, nullptr, clk) {

	XMLNode* n = getNode(antennaEl, "maxconnections");
	m_maxConnections = atoi(n->ToText()->Value());
	n = getNode(antennaEl, "power");
	m_power = atof(n->ToText()->Value());
	n = getNode(antennaEl, "attenuationfactor");
	m_ple = atof(n->ToText()->Value());
	n = getNode(antennaEl, "type");
	const char* t = n->ToText()->Value();
	m_type = AntennaType::OMNIDIRECTIONAL;
	if (!strcmp(t, "directional"))
		m_type = AntennaType::DIRECTIONAL;
	n = getNode(antennaEl, "Smid");
	m_Smid = atof(n->ToText()->Value());
	n = getNode(antennaEl, "SSteep");
	m_SSteep = atof(n->ToText()->Value());

	n = getNode(antennaEl, "x");
	double x = atof(n->ToText()->Value());
	n = getNode(antennaEl, "y");
	double y = atof(n->ToText()->Value());

	n = getNode(antennaEl, "height");
	if (n != nullptr)
		m_height = atof(n->ToText()->Value());
	else
		m_height = Constants::ANTENNA_HEIGHT;

	Coordinate c = Coordinate(x, y, m_height);
	Point* p = getMap()->getGlobalFactory()->createPoint(c);
	setLocation(p);

	n = getNode(antennaEl, "tilt");
	if (n != nullptr)
		m_tilt = atof(n->ToText()->Value());
	else
		m_tilt = Constants::ANTENNA_TILT;

	if (m_type == AntennaType::DIRECTIONAL) {
		n = getNode(antennaEl, "azim_dB_back");
		if (n != nullptr)
			m_azim_dB_Back = atof(n->ToText()->Value());
		else
			m_azim_dB_Back = Constants::ANTENNA_AZIM_DB_BACK;

		n = getNode(antennaEl, "elev_dB_back");
		if (n != nullptr)
			m_elev_dB_Back = atof(n->ToText()->Value());
		else
			m_elev_dB_Back = Constants::ANTENNA_ELEV_DB_BACK;

		n = getNode(antennaEl, "beam_h");
		if (n != nullptr)
			m_beam_H = atof(n->ToText()->Value());
		else
			m_beam_H = Constants::ANTENNA_BEAM_H;

		n = getNode(antennaEl, "beam_v");
		if (n != nullptr)
			m_beam_V = atof(n->ToText()->Value());
		else
			m_beam_V = Constants::ANTENNA_BEAM_V;

		n = getNode(antennaEl, "direction");
		if (n != nullptr)
			m_direction = atof(n->ToText()->Value());
		else
			m_direction = Constants::ANTENNA_DIRECTION;

	}
	string fileName = getName() + std::to_string(id) + ".csv";
	try {
		m_file.open(fileName, ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		cerr << "Output goes to the console!" << endl;
	}
	m_S0 = 30 + 10 * log10(m_power);
	m_cell = this->getMap()->getGlobalFactory()->createPolygon();
}

Antenna::~Antenna() {
	if (m_file.is_open()) {
		try {
			m_file.close();
		} catch (std::ofstream::failure& e) {
			cerr << "Error closing output files!" << endl;
		}
	}
}

double Antenna::getPLE() const {
	return (m_ple);
}

void Antenna::setPLE(double ple) {
	m_ple = ple;
}

const string Antenna::toString() const {
	ostringstream result;
	result << ImmovableAgent::toString() << left << setw(15) << m_power << setw(25) << m_maxConnections << setw(15) << m_ple;
	return (result.str());
}

double Antenna::getPower() const {
	return (m_power);
}

void Antenna::setPower(double power) {
	m_power = power;
}

unsigned long Antenna::getMaxConnections() const {
	return (m_maxConnections);
}

void Antenna::setMaxConnections(int maxConnections) {
	m_maxConnections = maxConnections;
}

bool Antenna::tryRegisterDevice(HoldableAgent* device) {
	bool result = false;
	if (/*m_numActiveConnections*/getNumActiveConections() < m_maxConnections) {
		//if the device is not yet registered
		if (!alreadyRegistered(device)) {
			attachDevice(device);
			result = true;
		} else {
			registerEvent(device, EventType::ALREADY_ATTACHED_DEVICE, false);
			result = true;
		}
	} else {
		registerEvent(device, EventType::IN_RANGE_NOT_ATTACHED_DEVICE, false);
	}

	return (result);
}

void Antenna::attachDevice(HoldableAgent* device) {
	m_devices.push_back(device);
	registerEvent(device, EventType::ATTACH_DEVICE, false);
//m_numActiveConnections++;
}

void Antenna::dettachDevice(HoldableAgent* device) {
	vector<HoldableAgent*>::iterator it = std::find(m_devices.begin(), m_devices.end(), device);
	if (it != m_devices.end()) {
		m_devices.erase(it);
		//m_numActiveConnections--;
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

void Antenna::setType(AntennaType type) {
	m_type = type;
}

unsigned long Antenna::getNumActiveConections() {
	return (m_devices.size());
}

void Antenna::registerEvent(HoldableAgent * ag, const EventType event, const bool verbose) {
	char sep = Constants::sep;
	if (verbose) {
		cout << " Time: " << getClock()->getCurrentTime() << sep << " Antenna id: " << getId() << sep << " Event registered for device: " << ag->getId() << sep;
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
		cout << sep << " Location: " << ag->getLocation()->getCoordinate()->x << sep << ag->getLocation()->getCoordinate()->y;
		cout << endl;
	} else {
		stringstream ss;
		ss << getClock()->getCurrentTime() << sep << getId() << sep << static_cast<int>(event) << sep << ag->getId() << sep << ag->getLocation()->getCoordinate()->x << sep
				<< ag->getLocation()->getCoordinate()->y << endl;

		if (m_file.is_open()) {
			m_file << ss.str();
			m_file.flush();
		} else
			cout << ss.str();
	}
}

double Antenna::S0() const {
	return m_S0;
	//return (30 + 10 * log10(m_power));
}

double Antenna::SDist(double dist) const {
	return (10 * m_ple * log10(dist));
}

double Antenna::S(double dist) const {
	if (m_type == AntennaType::OMNIDIRECTIONAL)
		return (S0() - SDist(dist));
	else
		throw std::runtime_error("Unsupported antenna type (yet)! ");
}

double Antenna::getSmid() const {
	return (m_Smid);
}

void Antenna::setSmid(double smid) {
	m_Smid = smid;
}

double Antenna::getSSteep() const {
	return (m_SSteep);
}

void Antenna::setSSteep(double sSteep) {
	m_SSteep = sSteep;
}

double Antenna::computeSignalQuality(const Point* p) const {
	double result = 0.0;
	double signalStrength = S(p->distance(getLocation()));
	if (m_type == AntennaType::OMNIDIRECTIONAL) {
		result = computeSignalQualityOmnidirectional(p);
//		if (signalStrength > -100)
//			result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
//		else
//			result = 0.0;
	}
	return (result);
}

double Antenna::computeSignalQuality(const Coordinate c) const {
	double result;
	Point* p = getMap()->getGlobalFactory()->createPoint(c);
	result = computeSignalQuality(p);
	getMap()->getGlobalFactory()->destroyGeometry(p);
	return result;
}


double Antenna::computeSignalQualityOmnidirectional(const Point* p) const {
	double result = 0.0;
//	double x = p->getCoordinate()->x;
//	double y = p->getCoordinate()->y;
//    double antennaX = getLocation()->getCoordinate()->x;
//    double antennaY = getLocation()->getCoordinate()->y;
    double dist = p->distance(getLocation());
    //double distXY = sqrt( pow(x-antennaX, 2) + pow(y-antennaY, 2));
    double signalStrength = S(dist);
    result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
	return result;
}


double Antenna::computeSignalQualityDirectional(const Point* p) const {
	double result = 0.0;
//	double x = p->getCoordinate()->x;
//	double y = p->getCoordinate()->y;
//    double antennaX = getLocation()->getCoordinate()->x;
//    double antennaY = getLocation()->getCoordinate()->y;
//    double dist = p->distance(getLocation());
//    double distXY = sqrt( pow(x-antennaX, 2) + pow(y-antennaY, 2));
//
//
//
//
//    double signalStrength = S(dist);
//
//
//
//
//    result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
	return result;
}



double Antenna::computePower(const Point* p) const {
	double result = 0.0;
	if (m_type == AntennaType::OMNIDIRECTIONAL)
		result = m_power * pow(p->distance(getLocation()), -m_ple);

	return (result);
}

const string Antenna::getName() const {
	return ("Antenna");
}


double Antenna::getAzimDBBack() const {
	return m_azim_dB_Back;
}

void Antenna::setAzimDBBack(double azimDBBack) {
	m_azim_dB_Back = azimDBBack;
}

double Antenna::getBeamH() const {
	return m_beam_H;
}

void Antenna::setBeamH(double beamH) {
	m_beam_H = beamH;
}

double Antenna::getBeamV() const {
	return m_beam_V;
}

void Antenna::setBeamV(double beamV) {
	m_beam_V = beamV;
}

double Antenna::getElevDBBack() const {
	return m_elev_dB_Back;
}

void Antenna::setElevDBBack(double elevDBBack) {
	m_elev_dB_Back = elevDBBack;
}

double Antenna::getHeight() const {
	return m_height;
}

void Antenna::setHeight(double height) {
	m_height = height;
}

double Antenna::getTilt() const {
	return m_tilt;
}

void Antenna::setTilt(double tilt) {
	m_tilt = tilt;
}

void Antenna::setLocationWithElevation() {
	Point* p  = getLocation();
	Point* newP = getMap()->getGlobalFactory()->createPoint(Coordinate(p->getCoordinate()->x, p->getCoordinate()->y, m_height ));
	setLocation(newP);
	getMap()->getGlobalFactory()->destroyGeometry(p);
}

double Antenna::getDirection() const {
	return m_direction;
}

void Antenna::setDirection(double direction) {
	m_direction = direction;
}
