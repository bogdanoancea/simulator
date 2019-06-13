/*
 * Data simulator for mobile phone network events
 *
 * Antenna.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef ANTENNA_H_
#define ANTENNA_H_

#include <ImmovableAgent.h>
#include <HoldableAgent.h>
#include <geos/geom/Polygon.h>
#include <EventType.h>
#include <AntennaType.h>
#include <TinyXML2.h>
#include <string>
#include <fstream>
#include <utility>


using namespace tinyxml2;
using namespace geos;
using namespace geos::geom;

/**
 * This class simulates an antenna of the mobile phone network.
 */
class Antenna: public ImmovableAgent {
public:

	/**
	 * Constructor of the class. It builds an object providing directly the values for each parameter of the antenna.
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
	explicit Antenna(const Map* m, const unsigned long id, Point* initPosition, const Clock* clock, double attenuationFactor, double power,
			unsigned long maxConnections, double smid, double ssteep, AntennaType type);

	/**
	 * Constructor of the class. It builds an object taking the value of the antenna' parameters from an XML Element,
	 * usually when an Antenna object is built reading the xml configuration file.
	 * @param m a pointer to the Map object used for the simulation
	 * @param clock a pointer to the Clock object used for the simulation
	 * @param id the id of the Antenna
	 * @param el the XML Element containing the parameters of the Antenna.
	 */
	explicit Antenna(const Map* m, const Clock* clock, const unsigned long id, XMLElement* el);


	/**
	 * Destructor of the class. It closes the file where the Antenna dumps the registered events during the simulation.
	 */
	virtual ~Antenna();


	/**
	 * Overrides the same method from the superclass.
	 * @return the name of the class, i.e. "Antenna"
	 */
	const string getName() const override;

	/**
	 * Overrides the same method from the superclass. It is used to write the characteristics of the Antenna in a file or console.
	 * @return a string that describes the parameters of the Antenna.
	 */
	const string toString() const override;

	/**
	 * Returns the surrounding environment' path loss exponent of the signal.
	 * @return the signals' path loss exponent of the surrounding environment. In real life, it takes
	 * values between 2 (in open field) and 6 (inside buildings).
	 */
	double getPLE() const;

	/**
	 * Sets the surrounding environment' path loss exponent of the signal for an antenna.
	 * @param ple the value of the surrounding environment' path loss exponent of the signal. In real life, it takes
	 * values between 2 (in open field) and 6 (inside buildings).
	 */
	void setPLE(double ple);

	/**
	 * Returns the power of an antenna in Watts at the location of antenna. This power decreases with a power of the distance from antenna.
	 * @return the power of an antenna in Watts.
	 */
	double getPower() const;

	/**
	 * Sets the power of an antenna.
	 * @param power the value of the antenna's power.
	 */
	void setPower(double power);

	/**
	 * Returns the maximum number of mobile devices that an antenna can connect.
	 */
	unsigned long getMaxConnections() const;

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

	/**
	 * Sets the antenna type.
	 * @param type the antenna type. It could take the following two values: AntennaType::OMNIDIRECTIONAL or AntennaType::DIRECTIONAL.
	 */
	void setType(AntennaType type);


	/**
	 * Computes the signal strength at the distance dist from antenna location.
	 * @param dist the distance from antenna location.
	 * @return the signal strength.
	 */
	double S(double dist) const;


	/**
	 * Returns the value of the Smid antenna parameter.
	 * @return the value of the Smid antenna parameter.
	 */
	double getSmid() const;

	/**
	 * Sets the value of the Smid antenna parameter.
	 * @param smid the value of the Smid antenna parameter.
	 */
	void setSmid(double smid);


	/**
	 * Returns the value of the Ssteep antenna parameter.
	 * @return the value of the Ssteep antenna parameter.
	 */
	double getSSteep() const;

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
	 * @param c coordinates of the location where we want to compute the signal quality.
	 * @return the signal quality.
	 */
	double computeSignalQuality(const Coordinate c) const;

	/**
	 * Computes the power of the signal given by an antenna in a certain location.
	 * @param p the location where we want to compute the power of the signal.
	 * @return the power of the signal in the location given by Point p.
	 */
	double computePower(const Point* p) const;


	double getAzimDBBack() const;
	void setAzimDBBack(double azimDBBack);
	double getBeamH() const;
	void setBeamH(double beamH);
	double getBeamV() const;
	void setBeamV(double beamV);
	double getElevDBBack() const;
	void setElevDBBack(double elevDBBack);
	double getHeight() const;
	void setHeight(double height);
	double getTilt() const;
	void setTilt(double tilt);
	double getDirection() const;
	void setDirection(double direction);

private:

	bool alreadyRegistered(HoldableAgent * ag);
	void registerEvent(HoldableAgent * ag, const EventType event, const bool verbose);
	unsigned long getNumActiveConections();
	double S0()const;
	double SDist(double dist) const;
	double computeSignalQualityOmnidirectional(const Point* p) const;
	double computeSignalQualityDirectional(const Point* p) const;
	void setLocationWithElevation();
	double projectToEPlane(double b, double c, double beta) const;
	vector<pair<double, double>> createMapping(double dbBack) const;
	double getMmin3db(double sd, double dbBack) const;

	double m_ple;
	double m_power;
	unsigned long m_maxConnections;
	double m_Smid;
	double m_SSteep;

	Polygon* m_cell;
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

};

#endif /* ANTENNA_H_ */
