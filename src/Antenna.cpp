#include <Antenna.h>
#include <HoldableAgent.h>
#include <geos/geom/GeometryFactory.h>
#include <Map.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <EventType.h>
#include <Constants.h>

using namespace std;

Antenna::Antenna(Map* m, long id, Point* initPosition, Clock* clock, double attenuationFactor, double power, unsigned long maxConnections,
		AntennaType type) :
		ImmovableAgent(m, id, initPosition, clock), m_power { power }, m_attenuationFactor { attenuationFactor }, m_maxConnections {
				maxConnections }, /*m_numActiveConnections { 0 },*/m_type { type } {
	m_cell = this->getMap()->getGlobalFactory()->createPolygon();
}

Antenna::~Antenna() {
	// TODO Auto-generated destructor stub
}

double Antenna::getAttenuationFactor() const {
	return m_attenuationFactor;
}

void Antenna::setAttenuationFactor(double attenuationFactor) {
	m_attenuationFactor = attenuationFactor;
}

Polygon* Antenna::getCell() const {
	return m_cell;
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
	return m_power;
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
	return m_type;
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
		cout << getClock()->getCurrentTime() << sep << getId() << sep << code << sep << ag->getLocation()->getCoordinate()->x << sep
				<< ag->getLocation()->getCoordinate()->y;
		cout << endl;
	}

}
