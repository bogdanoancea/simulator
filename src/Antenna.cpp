#include <Antenna.h>
#include <HoldableAgent.h>
#include <geos/geom/GeometryFactory.h>
#include <Map.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <EventType.h>
#include <Constants.h>
#include <string.h>
#include <tinyxml2.h>
#include <Utils.h>

using namespace tinyxml2;
using namespace std;
using namespace utils;

Antenna::Antenna(Map* m, long id, Point* initPosition, Clock* clock, double attenuationFactor, double power, unsigned long maxConnections,
		double smid, double ssteep, AntennaType type) :
		ImmovableAgent(m, id, initPosition, clock), m_attenuationFactor { attenuationFactor }, m_power { power }, m_maxConnections {
				maxConnections }, m_Smid { smid }, m_SSteep { ssteep }, m_type { type } {

	string fileName = getName() + std::to_string(id) + ".csv";
	try {
		m_file.open(fileName, ios::out);
	}
	catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		cerr << "Output goes to the console!" << endl;
	}

	m_cell = this->getMap()->getGlobalFactory()->createPolygon();
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
}

double Antenna::getAttenuationFactor() const {
	return (m_attenuationFactor);
}

void Antenna::setAttenuationFactor(double attenuationFactor) {
	m_attenuationFactor = attenuationFactor;
}

Polygon * Antenna::getCell() const {
	return (m_cell);
}

void Antenna::setCell(const Polygon& cell) {
//m_cell = cell;
}

string Antenna::toString() {
	ostringstream result;
	result << ImmovableAgent::toString() << left << setw(15) << m_power << setw(25) << m_maxConnections << setw(15) << m_attenuationFactor;
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

void Antenna::registerEvent(HoldableAgent * ag, EventType event, bool verbose) {
	char sep = Constants::sep;
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
		cout << sep << " Location: " << ag->getLocation()->getCoordinate()->x << sep << ag->getLocation()->getCoordinate()->y;
		cout << endl;
	}
	else {
		int code = -1;
		switch (event) {
			case EventType::ATTACH_DEVICE:
				code = 1;
				break;
			case EventType::DETACH_DEVICE:
				code = 2;
				break;
			case EventType::ALREADY_ATTACHED_DEVICE:
				code = 3;
				break;
			case EventType::IN_RANGE_NOT_ATTACHED_DEVICE:
				code = 4;
		}
		stringstream ss;
		ss << getClock()->getCurrentTime() << sep << getId() << sep << code << sep << ag->getId() << sep
				<< ag->getLocation()->getCoordinate()->x << sep << ag->getLocation()->getCoordinate()->y << endl;

		if (m_file.is_open())
			m_file << ss.str();
		else
			cout << ss.str();
	}
}

double Antenna::S0() {
	return (30 + 10 * log10(m_power));
}

double Antenna::SDist(double dist) {
	return (10 * m_attenuationFactor * log10(dist));
}

double Antenna::S(double dist) {
	if (m_type == AntennaType::OMNIDIRECTIONAL)
		return (S0() - SDist(dist));
	else
		throw std::runtime_error("Unsupported antenna type (yet)! ");
}

double Antenna::getSmid() const {
	return m_Smid;
}

void Antenna::setSmid(double smid) {
	m_Smid = smid;
}

double Antenna::getSSteep() const {
	return m_SSteep;
}

void Antenna::setSSteep(double sSteep) {
	m_SSteep = sSteep;
}
