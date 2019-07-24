#include <Antenna.h>
#include <HoldableAgent.h>
#include <EventType.h>
#include <Constants.h>
#include <geos/geom/GeometryFactory.h>
#include <geos/util/GeometricShapeFactory.h>
#include <geos/io/WKTWriter.h>
#include <Map.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <string>
#include <string.h>
#include <TinyXML2.h>
#include <Utils.h>
#include <utility>
#include <RandomNumberGenerator.h>
#include <EMField.h>

using namespace tinyxml2;
using namespace std;
using namespace utils;

Antenna::Antenna(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double attenuationFactor, double power, unsigned long maxConnections, double smid,
		double ssteep, AntennaType type) :
		ImmovableAgent(m, id, initPosition, clock), m_ple { attenuationFactor }, m_power { power }, m_maxConnections { maxConnections }, m_Smid { smid }, m_SSteep { ssteep }, m_type {
				type }, m_height { Constants::ANTENNA_HEIGHT }, m_tilt { Constants::ANTENNA_TILT } {

	string fileName = getAntennaOutputFileName();
	try {
		m_file.open(fileName, ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		cerr << "Output goes to the console!" << endl;
	}
	m_S0 = 30 + 10 * log10(m_power);
	if (type == AntennaType::DIRECTIONAL) {
		m_beam_V = Constants::ANTENNA_BEAM_V;
		m_beam_H = Constants::ANTENNA_BEAM_H;
		m_azim_dB_Back = Constants::ANTENNA_AZIM_DB_BACK;
		m_elev_dB_Back = Constants::ANTENNA_ELEV_DB_BACK;
		m_direction = Constants::ANTENNA_DIRECTION;
	}
	setLocationWithElevation();
	m_cell = this->getMap()->getGlobalFactory()->createPolygon();
}

Antenna::Antenna(const Map* m, const Clock* clk, const unsigned long id, XMLElement* antennaEl, vector<MobileOperator*> mnos) :
		ImmovableAgent(m, id, nullptr, clk) {

	XMLNode* n = getNode(antennaEl, "mno_name");
	const char* mno_name = n->ToText()->Value();
	for (unsigned int i = 0; i < mnos.size(); i++) {
		if (mnos.at(i)->getMNOName().compare(mno_name) == 0) {
			m_MNO = mnos.at(i);
		}
	}
	n = getNode(antennaEl, "maxconnections");
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

	n = getNode(antennaEl, "Smin");
	m_Smin = atof(n->ToText()->Value());

	n = getNode(antennaEl, "qual_min");
	m_minQuality = atof(n->ToText()->Value());

	n = getNode(antennaEl, "Smid");
	m_Smid = atof(n->ToText()->Value());
	n = getNode(antennaEl, "SSteep");
	m_SSteep = atof(n->ToText()->Value());

	n = getNode(antennaEl, "x");
	double x = atof(n->ToText()->Value());
	n = getNode(antennaEl, "y");
	double y = atof(n->ToText()->Value());

	n = getNode(antennaEl, "z");
	if (n != nullptr)
		m_height = atof(n->ToText()->Value());
	else
		m_height = Constants::ANTENNA_HEIGHT;
//TODO get elevation from Grid
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
	string fileName = getAntennaOutputFileName();
	try {
		m_file.open(fileName, ios::out);
	} catch (std::ofstream::failure& e) {
		cerr << "Error opening output files!" << endl;
		cerr << "Output goes to the console!" << endl;
	}
	m_S0 = 30 + 10 * log10(m_power);
	m_rmax = pow(10, (3 - m_Smin / 10) / m_ple) * pow(m_power, 10 / m_ple);
//	cout << m_rmax << "," << getId() << endl;
	geos::util::GeometricShapeFactory shapefactory(this->getMap()->getGlobalFactory().get());
	shapefactory.setCentre(Coordinate(x, y));
	shapefactory.setSize(m_rmax);
	m_cell = shapefactory.createCircle();
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
	result << ImmovableAgent::toString() << left << setw(15) << m_power << setw(25) << m_maxConnections << setw(15) << m_ple << setw(15) << m_MNO->getId();
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
}

double Antenna::SDist(double dist) const {
	return (10 * m_ple * log10(dist));
}

double Antenna::S(double dist) const {
	//if (m_type == AntennaType::OMNIDIRECTIONAL)
	return (S0() - SDist(dist));
	//else
	//	throw std::runtime_error("Unsupported antenna type (yet)! ");
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
	const Coordinate* c = p->getCoordinate();
	if (m_type == AntennaType::OMNIDIRECTIONAL) {
		result = computeSignalQualityOmnidirectional(*c);
	}
	if (m_type == AntennaType::DIRECTIONAL) {
		result = computeSignalQualityDirectional(*c);
	}
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

double Antenna::computeSignalQualityOmnidirectional(const Coordinate c) const {
	double result = 0.0;
	Point *p = getLocation();
	const Coordinate* cc = p->getCoordinate();
	double dist = sqrt((c.z - cc->z) * (c.z - cc->z) + (c.y - cc->y) * (c.y - cc->y) + (c.x - cc->x) * (c.x - cc->x));
	double signalStrength = S(dist);
	result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
	return result;
}

double Antenna::computeSignalQualityOmnidirectional(const Point* p) const {
	double result = 0.0;
	const Coordinate* c = p->getCoordinate();
	result = computeSignalQualityOmnidirectional(*c);
	return result;
}

double Antenna::computeSignalQualityDirectional(const Point* p) const {
	double result = 0.0;
	double signalStrength = 0.0;
	double x = p->getCoordinate()->x;
	double y = p->getCoordinate()->y;
	double z = p->getCoordinate()->z;

	double antennaX = getLocation()->getCoordinate()->x;
	double antennaY = getLocation()->getCoordinate()->y;
	double antennaZ = getLocation()->getCoordinate()->z;

	double dist = p->distance(getLocation());
	double distXY = sqrt(pow(x - antennaX, 2) + pow(y - antennaY, 2));
	signalStrength += S(dist);

	double theta_azim = 90 - r2d(atan2(y - antennaY, x - antennaX));
	if (theta_azim < 0)
		theta_azim += 360;

	double azim = fmod(theta_azim - m_direction, 360);
	if (azim > 180)
		azim -= 360;
	if (azim < -180)
		azim += 360;

//project azim to elevation plane -> azim2
	double r_azim = d2r(azim);
	double a = sin(r_azim) * distXY;
	double b = cos(r_azim) * distXY;

	double e = projectToEPlane(b, m_height - z, m_tilt);
	double azim2 = r2d(atan2(a, e));
	vector<pair<double, double>> mapping = createMapping(m_azim_dB_Back);
//	for (int i = 0; i < mapping.size(); i++) {
//		cout << mapping[i].first << "," << mapping[i].second << endl;
//	}
	double sd = findSD(m_beam_H, m_azim_dB_Back, mapping);

	signalStrength += norm_dBLoss(azim2, m_azim_dB_Back, sd);

//vertical component
	double gamma_elevation = r2d(atan2(antennaZ - z, distXY));
	double elevation = (static_cast<int>(gamma_elevation - m_tilt)) % 360;
	if (elevation > 180)
		elevation -= 360;
	if (elevation < -180)
		elevation += 360;

	sd = findSD(m_beam_V, m_elev_dB_Back, mapping);  //? oare e acelasi mapping?
	signalStrength += norm_dBLoss(elevation, m_elev_dB_Back, sd);

	result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
	return result;
}

double Antenna::computeSignalQualityDirectional(const Coordinate c) const {
	double result = 0.0;
	double signalStrength = 0.0;
	double x = c.x;
	double y = c.y;
	double z = c.z;

	double antennaX = getLocation()->getCoordinate()->x;
	double antennaY = getLocation()->getCoordinate()->y;
	double antennaZ = getLocation()->getCoordinate()->z;

	double dist = sqrt((x - antennaX) * (x - antennaX) + (y - antennaY) * (y - antennaY));  //p->distance(getLocation());
	double distXY = sqrt(pow(x - antennaX, 2) + pow(y - antennaY, 2));
	signalStrength += S(dist);

	double theta_azim = 90 - r2d(atan2(y - antennaY, x - antennaX));
	if (theta_azim < 0)
		theta_azim += 360;

	double azim = fmod(theta_azim - m_direction, 360);
	if (azim > 180)
		azim -= 360;
	if (azim < -180)
		azim += 360;

//project azim to elevation plane -> azim2
	double r_azim = d2r(azim);
	double a = sin(r_azim) * distXY;
	double b = cos(r_azim) * distXY;

	double e = projectToEPlane(b, m_height - z, m_tilt);
	double azim2 = r2d(atan2(a, e));
	vector<pair<double, double>> mapping = createMapping(m_azim_dB_Back);
//	for (int i = 0; i < mapping.size(); i++) {
//		cout << mapping[i].first << "," << mapping[i].second << endl;
//	}
	double sd = findSD(m_beam_H, m_azim_dB_Back, mapping);

	signalStrength += norm_dBLoss(azim2, m_azim_dB_Back, sd);

//vertical component
	double gamma_elevation = r2d(atan2(antennaZ - z, distXY));
	double elevation = (static_cast<int>(gamma_elevation - m_tilt)) % 360;
	if (elevation > 180)
		elevation -= 360;
	if (elevation < -180)
		elevation += 360;

	sd = findSD(m_beam_V, m_elev_dB_Back, mapping);  //? oare e acelasi mapping?
	signalStrength += norm_dBLoss(elevation, m_elev_dB_Back, sd);

	result = 1.0 / (1 + exp(-m_SSteep * (signalStrength - m_Smid)));
	return result;
}

//TODO
double Antenna::findSD(double beamWidth, double dbBack, vector<pair<double, double>> mapping) const {
	double result = 0.0;
	vector<double> tmp;
	for (auto& i : mapping) {
		tmp.push_back(i.second - beamWidth / 2.0);
	}
	int indexMin = std::min_element(tmp.begin(), tmp.end()) - tmp.begin();
	result = mapping[indexMin].first;
	return result;
}

vector<pair<double, double>> Antenna::createMapping(double dbBack) const {
	vector<pair<double, double>> v;
	vector<pair<double, double>> result;
	double* sd = EMField::instance()->getSd();
	for (unsigned int i = 0; i < Constants::ANTENNA_MAPPING_N; i++) {
		double deg = getMin3db(sd[i], dbBack);
		pair<double, double> p = make_pair(sd[i], deg);
		v.push_back(p);
		//cout << " sd " << sd << " deg " << deg << endl;
	}
	for (unsigned int i = 1; i <= 180; i++) {
		double sd = searchMin(i, v);
		pair<double, double> p = make_pair(i, sd);
		result.push_back(p);
	}
	return result;
}

double Antenna::searchMin(double dg, vector<pair<double, double>> _3dBDegrees) const {
	for (pair<double, double>& i : _3dBDegrees) {
		i.second -= dg;
		i.second = fabs(i.second);
	}
	int minElementIndex = std::min_element(begin(_3dBDegrees), end(_3dBDegrees), [](const pair<double, double>& a, const pair<double, double>& b) {
		return a.second < b.second;
	}) - begin(_3dBDegrees);

	return _3dBDegrees[minElementIndex].first;
}

double Antenna::getMin3db(double sd, double dbBack) const {
	vector<double> v;
	const double* p1 = EMField::instance()->getAntennaMin3DbArray();
	for (unsigned int i = 0; i < Constants::ANTENNA_MIN_3_DB; i++) {
		//double p1 = i * 180.0 / (Constants::ANTENNA_MIN_3_DB - 1.0);
		double p2 = fabs(-3.0 - norm_dBLoss(p1[i], dbBack, sd));
		v.push_back(p2);
	}
	int minElementIndex = std::min_element(v.begin(), v.end()) - v.begin();
	return (minElementIndex * 180.0 / (Constants::ANTENNA_MIN_3_DB - 1));
}

double Antenna::norm_dBLoss(double angle, double dbBack, double sd) const {
	double a = normalizeAngle(angle);
	RandomNumberGenerator* rand = RandomNumberGenerator::instance(0);
	double inflate = -dbBack / (rand->normal_pdf(0.0, 0.0, sd) - rand->normal_pdf(180.0, 0.0, sd));
	return ((rand->normal_pdf(a, 0.0, sd) - rand->normal_pdf(0.0, 0.0, sd)) * inflate);
}

double Antenna::normalizeAngle(double angle) const {
	double a = fmod(angle, 360);
	if (a > 180)
		a = 360 - a;
	return a;
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
	Point* p = getLocation();
	Point* newP = getMap()->getGlobalFactory()->createPoint(Coordinate(p->getCoordinate()->x, p->getCoordinate()->y, m_height));
	setLocation(newP);
	getMap()->getGlobalFactory()->destroyGeometry(p);
}

double Antenna::getDirection() const {
	return m_direction;
}

void Antenna::setDirection(double direction) {
	m_direction = direction;
}

MobileOperator* Antenna::getMNO() const {
	return m_MNO;
}

void Antenna::setMNO(MobileOperator* mno) {
	m_MNO = mno;
}

string Antenna::getAntennaOutputFileName() const {
	return getName() + std::to_string(getId()) + "_MNO_" + m_MNO->getMNOName() + ".csv";
}

double Antenna::getRmax() const {
	return m_rmax;
}

double Antenna::getSmin() const {
	return m_Smin;
}

string Antenna::dumpCell() const {
	geos::io::WKTWriter writter;
	ostringstream result;
	result << writter.write(m_cell->getBoundary()) << endl;
	return result.str();
}
