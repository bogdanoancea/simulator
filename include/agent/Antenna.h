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
#include <EventType.h>
#include <geos/geom/Coordinate.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace tinyxml2 {
	class XMLElement;
} /* namespace tinyxml2 */

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
		explicit Antenna(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double attenuationFactor,
				double power, unsigned long maxConnections, double smid, double ssteep, AntennaType type);

		/**
		 * Constructor of the class. It builds an object taking the value of the antenna' parameters from an XML Element,
		 * usually when an Antenna object is built reading the xml configuration file.
		 * @param m a pointer to the Map object used for the simulation
		 * @param clock a pointer to the Clock object used for the simulation
		 * @param id the id of the Antenna
		 * @param el the XML Element containing the parameters of the Antenna
		 * @param mnos a vector with pointers to MobileOperator objects.
		 */
		explicit Antenna(const Map* m, const Clock* clock, const unsigned long id, XMLElement* el, vector<MobileOperator*> mnos);

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

		//TODO remove it - never called
		/**
		 * Returns the surrounding environment' path loss exponent of the signal.
		 * @return the signals' path loss exponent of the surrounding environment. In real life, it takes
		 * values between 2 (in open field) and 6 (inside buildings).
		 */
		double getPLE() const;

		//TODO remove it - never called
		/**
		 * Sets the surrounding environment' path loss exponent of the signal for an antenna.
		 * @param ple the value of the surrounding environment' path loss exponent of the signal. In real life, it takes
		 * values between 2 (in open field) and 6 (inside buildings).
		 */
		void setPLE(double ple);

		//TODO remove it - never called
		/**
		 * Returns the power of an antenna in Watts at the location of antenna. This power decreases with a power of the distance from antenna.
		 * @return the power of an antenna in Watts.
		 */
		double getPower() const;

		//TODO remove it - never called
		/**
		 * Sets the power of an antenna.
		 * @param power the value of the antenna's power.
		 */
		void setPower(double power);

		//TODO remove it - never called
		/**
		 * Returns the maximum number of mobile devices that an antenna can connect.
		 */
		unsigned long getMaxConnections() const;

		//TODO remove it - never called
		/**
		 * Sets the number of mobile devices that an antenna can connect.
		 * @param maxConnections the number of mobile devices that an antenna can connect.
		 */
		void setMaxConnections(int maxConnections);

		/**
		 * Tries to register a mobile device as being connected to this antenna.
		 * @param device a pointer to the object that represents a mobile device.
		 * @return true if the connection is successful, false otherwise.
		 */
		bool tryRegisterDevice(HoldableAgent* device);

		/**
		 * Connects a new mobile device and outputs an event EventType::ATTACH_DEVICE in the output file. Internally,
		 * the antenna keeps a vector with all connected mobile devices.
		 * devices. When a new mobile device is connected it is added to this vector.
		 * @param device a pointer to the object that represents the mobile device connected to this antenna.
		 */
		void attachDevice(HoldableAgent* device);

		/**
		 * Disconnects a mobile device from the antenna and outputs an event EventType::DEATACH_DEVICE in the output file. Internally,
		 * the mobile device is removed from the vector of the connected mobile devices.
		 * @param device
		 */
		void dettachDevice(HoldableAgent* device);

		/**
		 * Returns the antenna type: omnidirectional or directional
		 * @return the antenna type : AntennaType::OMNIDIRECTIONAL or AntennaType::DIRECTIONAL.
		 */
		AntennaType getType() const;

		//TODO remove it - never called
		/**
		 * Sets the antenna type.
		 * @param type the antenna type. It could take the following two values: AntennaType::OMNIDIRECTIONAL or AntennaType::DIRECTIONAL.
		 */
		void setType(AntennaType type);

		//TODO make it private
		/**
		 * Computes the signal strength at the distance dist from antenna location.
		 * @param dist the distance from antenna location.
		 * @return the signal strength.
		 */
		double S(double dist) const;

		//TODO remove it - never called
		/**
		 * Returns the value of the Smid antenna parameter.
		 * @return the value of the Smid antenna parameter.
		 */
		double getSmid() const;

		//TODO remove it - never called
		/**
		 * Sets the value of the Smid antenna parameter.
		 * @param smid the value of the Smid antenna parameter.
		 */
		void setSmid(double smid);

		//TODO remove it - never called
		/**
		 * Returns the value of the Ssteep antenna parameter.
		 * @return the value of the Ssteep antenna parameter.
		 */
		double getSSteep() const;

		//TODO remove it - never called
		/**
		 * Sets the value of the Ssteep antenna parameter.
		 * @param sSteep the value of the Ssteep antenna parameter.
		 */
		void setSSteep(double sSteep);

		/**
		 * Computes the signal quality given by an antenna in a certain location.
		 * @param p the location where we want to compute the signal quality.
		 * @return the signal quality.
		 */
		double computeSignalQuality(const Point* p) const;

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
		double computePower(const Point* p) const;

		/**
		 * Computes the power of the signal given by an antenna in a certain location.
		 * @param c the location where we want to compute the power of the signal.
		 * @return the power of the signal in the location given by Coordinate c.
		 */
		double computePower(const Coordinate c) const;

		//TODO make it private
		/**
		 * Computes the coverage area of an antenna. It is defined as the area where the signal strength is greater than S_min
		 * @return a Polygon* representing the coverage area of the antenna.
		 */
		Geometry* getCoverageArea();

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

		//TODO remove it - never called
		/**
		 * Returns the value of the minimum signal strength that defines the coverage area of this antenna.
		 * @return the value of the minimum signal strength that defines the coverage area of this antenna.
		 */
		double getSmin() const;

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
		double computeSignalStrength(const Point* p) const;

		//TODO make it private
		/**
		 * Computes the signal strength given by an antenna in a certain location.
		 * @param c the location where we want to compute the signal strength.
		 * @return the signal strength.
		 */
		double computeSignalStrength(const Coordinate c) const;

		//TODO make it private
		/**
		 * compute the signal strength, signal quality or signal power depending on the value of the handoverType parameter
		 * @param handoverType the handover mechanism: signal quality, signal strength, signal power
		 * @param c - a pointer to a coordinate that defines the location where the signal quality/strength/power should be computed
		 * @return the signal strength, signal quality or signal power depending on the value of the handoverType parameter
		 */
		double computeSignalMeasure(HoldableAgent::CONNECTION_TYPE handoverType, const Coordinate c) const;

		//TODO remove it - never called
		HoldableAgent::CONNECTION_TYPE getHandoverMechanism() const;

		void setHandoverMechanism(HoldableAgent::CONNECTION_TYPE handoverMechanism);

		void dumpSignal() const;

	private:

		bool alreadyRegistered(HoldableAgent * ag);
		void registerEvent(HoldableAgent * ag, const EventType event, const bool verbose);
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
		double findSD(double beamWidth, double dbBack, vector<pair<double, double>>& mapping) const;

		Geometry* getCoverageAreaOmnidirectional();
		Geometry* getCoverageAreaDirectional();

		double m_ple;
		double m_power;
		unsigned long m_maxConnections;
		double m_Smid;
		double m_SSteep;

		Geometry* m_cell;
		vector<HoldableAgent*> m_devices;
		AntennaType m_type;
		ofstream m_file;
		double m_S0;

		double m_height;
		double m_tilt;
		double m_beam_V;
		double m_beam_H;
		double m_azim_dB_Back;
		double m_elev_dB_Back;
		double m_direction;

		MobileOperator* m_MNO;

		double m_rmax;
		double m_Smin;
		double m_Qmin;
		vector<pair<double, double>> m_mapping_azim;
		vector<pair<double, double>> m_mapping_elev;
		double m_sd_azim;
		double m_sd_elev;

		HoldableAgent::CONNECTION_TYPE m_handoverMechanism;
};

#endif /* ANTENNA_H_ */
