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
 * Antenna.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <agent/HoldableAgent.h>
#include <agent/ImmovableAgent.h>
#include <agent/MobileOperator.h>
#include <AntennaType.h>
#include <events/EventFactory.h>
#include <events/EventType.h>
#include <geos/geom/Coordinate.h>
#include <NetworkType.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <parsers/AntennaConfiguration.h>
#include <parsers/SimConfigParser.h>


using namespace geos;
using namespace geos::geom;
using namespace tinyxml2;
/**
 * This class simulates an antenna of the mobile phone network.
 */
class Antenna: public ImmovableAgent {
public:

	/**
	 * Constructor of the class. It builds an object providing directly the values for each parameter of the antenna. This constructor
	 * is used only for testing purposes. For a real simulation the other constructor of the class should be used.
	 * @param m a pointer to the Map object used for the simulation
	 * @param id the id of the Antenna
	 * @param initPosition the position of the antenna on the map
	 * @param clock a pointer to the Clock object used for the simulation
	 * @param attenuationFactor the attenuation factor of the surrounding environment. In real life, it takes
	 * values between 2 (in open field) and 6 (inside buildings).
	 * @param power the power of the antenna in Watts.
	 * @param maxConnections the maximum number of the connections that the antenna can accept.
	 * @param smid is a parameter of an antenna. The significance of this parameter is described in mobloc R package.
	 * @param ssteep is a parameter of an antenna. The significance of this parameter is described in mobloc R package.
	 * @param type it could have two values AntennaType::OMNIDIRECTIONAL for omnidirectional antennas
	 * and AntennaType::DIRECTIONAL for directional antennas.
	 */
//	explicit Antenna(const Map *m, const unsigned long id, Point *initPosition,
//			const Clock *clock, double attenuationFactor, double power,
//			unsigned long maxConnections, double smid, double ssteep,
//			AntennaType type, string &outputDir, EventFactory &factory,
//			EventType evType);

	/**
	 * Constructor of the class. It builds an object taking the value of the antenna' parameters from an XML Element,
	 * usually when an Antenna object is built reading the xml configuration file.
	 * @param m a pointer to the Map object used for the simulation
	 * @param clock a pointer to the Clock object used for the simulation
	 * @param id the id of the Antenna
	 * @param el the XML Element containing the parameters of the Antenna
	 * @param mnos a vector with pointers to MobileOperator objects.
	 */
//	explicit Antenna(const Map *m, const Clock *clock, const unsigned long id,
//			XMLElement *el, vector<MobileOperator*> mnos,
//			const string &outptuDir, EventFactory &factory, EventType evType);

	explicit Antenna(const unsigned long id, SimConfigParser* sc, AntennaConfiguration ac, EventFactory* factory);

	/**
	 * Destructor of the class. It closes the file where the Antenna dumps the registered events during the simulation.
	 */
	virtual ~Antenna();

	/**
	 * Overrides the same method from the superclass. It is used to write the characteristics of the Antenna in a file or console.
	 * @return a string that describes the parameters of the Antenna.
	 */
	const string toString() const override;

	static const string getHeader();

	/**
	 * Tries to register a mobile device as being connected to this antenna.
	 * @param device a pointer to the object that represents a mobile device.
	 * @return true if the connection is successful, false otherwise.
	 */
	bool tryRegisterDevice(HoldableAgent *device);

	/**
	 * Connects a new mobile device and outputs an event EventType::ATTACH_DEVICE in the output file. Internally,
	 * the antenna keeps a vector with all connected mobile devices.
	 * devices. When a new mobile device is connected it is added to this vector.
	 * @param device a pointer to the object that represents the mobile device connected to this antenna.
	 */
	void attachDevice(HoldableAgent *device);

	/**
	 * Disconnects a mobile device from the antenna and outputs an event EventType::DEATACH_DEVICE in the output file. Internally,
	 * the mobile device is removed from the vector of the connected mobile devices.
	 * @param device a pointer  to the device to be disconnected.
	 */
	void dettachDevice(HoldableAgent *device);

	/**
	 * Returns the antenna type: omnidirectional or directional
	 * @return the antenna type : AntennaType::OMNIDIRECTIONAL or AntennaType::DIRECTIONAL.
	 */
	AntennaType getType() const;

	/**
	 * Computes the signal quality given by an antenna in a certain location.
	 * @param p the location where we want to compute the signal quality.
	 * @return the signal quality.
	 */
	double computeSignalQuality(const Point *p) const;

	/**
	 * Computes the signal quality given by an antenna in a certain location.
	 * @param c represents the coordinates of the location where we want to compute the signal quality.
	 * @return the signal quality.
	 */
	double computeSignalQuality(const Coordinate c) const;

	/**
	 * Computes the power of the signal given by an antenna in a certain location.
	 * @param p the location where we want to compute the power of the signal.
	 * @return the power of the signal in the location given by Point p.
	 */
	double computePower(const Point *p) const;

	/**
	 * Computes the power of the signal given by an antenna in a certain location.
	 * @param c the location where we want to compute the power of the signal.
	 * @return the power of the signal in the location given by Coordinate c.
	 */
	double computePower(const Coordinate c) const;

	/**
	 * Returns a pointer to an MNO object representing the MNO that own this antenna.
	 * @return a pointer to an MNO object representing the MNO that own this antenna.
	 */
	MobileOperator* getMNO() const;

	/**
	 * Builds the name of the output file where the events registered by this antenna during a simulation are saved.
	 * @return the name of the output file where the events registered by this antenna during a simulation are saved.
	 */
	string getAntennaOutputFileName() const;

	/**
	 * Computes the radius of the coverage area for an omnidirectional antenna. This area is a circle where the signal strength is greater than S_min.
	 * @return the radius of the coverage area for an omnidirectional antenna.
	 */
	double getRmax() const;

	/**
	 * Builds a wkt string that represents the coverage area of this antenna.
	 * @return a wkt string that represents the coverage area of this antenna.
	 */
	string dumpCell() const;

	/**
	 * Computes the signal strength given by an antenna in a certain location.
	 * @param p the location where we want to compute the signal strength.
	 * @return the signal strength.
	 */
	double computeSignalStrength(const Point *p) const;



	void dumpSignal() const;

	NetworkType getNetworkType();
	static string getEventHeader(EventType evType);
	unsigned long getMaxConnections() const;
	double getPower() const;
	double getAttenuationFactor() const;
	string getTypeName() const;
	double getSmin() const;
	double getQmin() const;
	double getSmid() const;
	double getSSteep() const;
	double getTilt() const;
	double getAzimDBBack() const;
	double getElevDBBack() const;
	double getBeamH() const;
	double getBeamV() const;
	double getDirection() const;
	double getHeight() const;

private:

	bool alreadyRegistered(HoldableAgent *ag);
	void registerEvent(HoldableAgent *ag, const EventCode event,
			const bool verbose);
	void registerEvent(Event *ev, Point *evtLocation);
	unsigned long getNumActiveConections();
	double S0() const;
	double SDist(double dist) const;
	double computeSignalQualityOmnidirectional(const Coordinate c) const;
	double computeSignalQualityDirectional(const Coordinate c) const;
	double computeSignalStrengthDirectional(const Coordinate c) const;
	double computeSignalStrengthOmnidirectional(const Coordinate c) const;

	void setLocationWithElevation();
	double projectToEPlane(double b, double c, double beta) const;
	vector<pair<double, double>> createMapping(double dbBack) const;
	double getMin3db(double sd, double dbBack) const;
	double norm_dBLoss(double angle, double dbBack, double sd) const;
	double normalizeAngle(double angle) const;
	double searchMin(double dg, vector<pair<double, double>> _3dBDegrees) const;
	double findSD(double beamWidth, double dbBack,
			vector<pair<double, double>> &mapping) const;

	Geometry* getCoverageAreaOmnidirectional() const;
	Geometry* getCoverageAreaDirectional() const;
	void setCell(HoldableAgent::CONNECTION_TYPE handoverMechanism);
	/**
	 * Computes the signal strength at the distance dist from antenna location.
	 * @param dist the distance from antenna location.
	 * @return the signal strength.
	 */
	double S(double dist) const;

	/**
	 * Computes the coverage area of an antenna. It is defined as the area where the signal strength is greater than S_min
	 * @return a Polygon* representing the coverage area of the antenna.
	 */
	Geometry* getCoverageArea();

	/**
	 * Computes the signal strength given by an antenna in a certain location.
	 * @param c the location where we want to compute the signal strength.
	 * @return the signal strength.
	 */
	double computeSignalStrength(const Coordinate c) const;

	/**
	 * compute the signal strength, signal quality or signal power depending on the value of the handoverType parameter
	 * @param handoverType the handover mechanism: signal quality, signal strength, signal power
	 * @param c - a pointer to a coordinate that defines the location where the signal quality/strength/power should be computed
	 * @return the signal strength, signal quality or signal power depending on the value of the handoverType parameter
	 */
	double computeSignalMeasure(HoldableAgent::CONNECTION_TYPE handoverType,
			const Coordinate c) const;

	EventConfig* buildEventConfig(EventType evType, EventCode code,
			HoldableAgent *device);

	Geometry *m_cell;
	vector<HoldableAgent*> m_devices;
	ofstream m_file;
	double m_S0;
	double m_rmax;
	vector<pair<double, double>> m_mapping_azim;
	vector<pair<double, double>> m_mapping_elev;
	double m_sd_azim;
	double m_sd_elev;

	EventFactory* m_eventFactory;

	AntennaConfiguration m_antennaConfig;
	SimConfigParser* m_simConfig;

	static const string EventHeaderCellID;
	static const string EventHeaderCellIDTA;
	static const double delta4G;
	static const double delta3G;
	static const unsigned int MAXTA4G;
	static const unsigned int MAXTA3G;
};

#endif /* ANTENNA_H_ */
