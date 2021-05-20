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
 * AntennaConfiguration.h
 *
 *  Created on: Apr 23, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_PARSERS_ANTENNACONFIGURATION_H_
#define INCLUDE_PARSERS_ANTENNACONFIGURATION_H_

#include <AntennaType.h>
#include <NetworkType.h>
#include <agent/MobileOperator.h>
#include <geos/geom/Point.h>


/**
 * A class that contains all the technical parameters needed to build an Antenna object. This object is build during the parsing
 * of the antenna configuration file and it is passed to the constructor of the Antenna object.
 */
class AntennaConfiguration {
public:
	/**
	 * The constructor of the class. It initializes all members of the class with the default values specified in the Constants class.
	 * While the configuration file is parsed, the setter methods are used to set the values of the corresponding parameter.
	 */
	AntennaConfiguration();

	/**
	 * The default destructor of the class.
	 */
	virtual ~AntennaConfiguration();

	/**
	 * Returns the difference in signal strength between front and back in the azimuth plane for directional antennas.
	 * @return the difference in signal strength between front and back in the azimuth plane for directional antennas.
	 */
	double getAzimDBBack() const;

	/**
	 * Returns the horizontal beam width for directional antennas.
	 * @return the horizontal beam width for directional antennas.
	 */
	double getBeamH() const;

	/**
	 * Returns the vertical beam width for directional antennas.
	 * @return the vertical beam width for directional antennas.
	 */
	double getBeamV() const;

	/**
	 * Returns the direction angle for directional antennas.
	 * @return the direction angle for directional antennas.
	 */
	double getDirection() const;

	/**
	 * * Returns the difference in signal strength between front and back in the elevation plane for directional antennas.
	 * @return the difference in signal strength between front and back in the elevation plane for directional antennas.
	 */
	double getElevDBBack() const;

	/**
	 * Returns the height of the antenna (the z coordinate of location of the antenna).
	 * @return the height of the antenna (the z coordinate of location of the antenna).
	 */
	double getHeight() const;

	/**
	 * Returns the location of the antenna on the map.
	 * @return the location of the antenna on the map.
	 */
	Point* getLocation() const;

	/**
	 * Returns the maximum number of simultaneous connections an antenna can accept.
	 * @return  the maximum number of simultaneous connections an antenna can accept.
	 */
	unsigned long getMaxConnections() const;

	/**
	 * Returns a pointer to the MobileOperator object that owns this antenna.
	 * @return a pointer to the MobileOperator object that owns this antenna.
	 */
	MobileOperator* getMno() const;

	/**
	 * Returns the network type of the antenna : NetworkType::_3G or NetworkType::_4G
	 * @return the network type of the antenna : NetworkType::_3G or NetworkType::_4G
	 */
	NetworkType getNetworkType() const;

	/**
	 * Returns the path loss exponent (the attenuation factor of the signal). This is a feature of the surrounding
	 * environment of the antenna.
	 * @return the path loss exponent (the attenuation factor of the signal). This is a feature of the surrounding
	 * environment of the antenna.
	 */
	double getPle() const;

	/**
	 * Returns the power of the antenna measured in Watts.
	 * @return the power of the antenna in measured Watts.
	 */
	double getPower() const;

	/**
	 * Returns the minimum value of the signal dominance that can be used to connect a mobile device.
	 * @return the minimum value of the signal dominance that can be used to connect a mobile device.
	 */
	double getQmin() const;

	/**
	 * Returns the Smid parameter of the signal propagation model. For details see
	 *
	 * Salgado, D., Sanguiao, L., Oancea, B. et al. An end-to-end statistical process with mobile network data for official statistics.
	 * EPJ Data Science, 10, 20 (2021). https://doi.org/10.1140/epjds/s13688-021-00275-w.
	 * @return the Smid parameter of the signal propagation model.
	 */
	double getSmid() const;

	/**
	 * Returns the minimum value of the signal strength that can be used to connect a mobile device.
	 * @return the minimum value of the signal strength that can be used to connect a mobile device.
	 */
	double getSmin() const;

	/**
	 * Returns the SSteep parameter of the signal propagation model. For details see
	 *
	 * Salgado, D., Sanguiao, L., Oancea, B. et al. An end-to-end statistical process with mobile network data for official statistics.
	 * EPJ Data Science, 10, 20 (2021). https://doi.org/10.1140/epjds/s13688-021-00275-w.
	 * @return the SSteep parameter of the signal propagation model.
	 */
	double getSSteep() const;

	/**
	 * Returns the tilt of the antenna.
	 * @return the tilt of the antenna.
	 */
	double getTilt() const;

	/**
	 * Returns the antenna type: AntennaType::DIRECTIONAL or AntennaType::OMNIDIRECTIONAL.
	 * @return the antenna type: AntennaType::DIRECTIONAL or AntennaType::OMNIDIRECTIONAL.
	 */
	AntennaType getType() const;

	/**
	 * Sets the difference in signal strength between front and back in the azimuth plane for directional antennas.
	 * @param azimDBBack the difference in signal strength between front and back in the azimuth plane for directional antennas.
	 */
	void setAzimDBBack(double azimDBBack);

	/**
	 * Sets the horizontal beam width for a directional antenna.
	 * @param beamH the horizontal beam width for a directional antenna.
	 */
	void setBeamH(double beamH);

	/**
	 * Sets the vertical beam width for a directional antenna.
	 * @param beamV the vertical beam width for a directional antenna.
	 */
	void setBeamV(double beamV);

	/**
	 * Sets the direction angle of a directional antenna. It is a number between 0 and 360.
	 * @param direction the direction angle of a directional antenna.
	 */
	void setDirection(double direction);

	/**
	 * Sets the difference in signal strength between front and back in the elevation plane for directional antennas.
	 * @param elevDBBack the difference in signal strength between front and back in the elevation plane for directional antennas.
	 */
	void setElevDBBack(double elevDBBack);

	/**
	 * Sets the height of the antenna (the z coordinate of location of the antenna).
	 * @param height the height of the antenna (the z coordinate of location of the antenna).
	 */
	void setHeight(double height);

	/**
	 * Sets the location of the antenna on the map.
	 * @param location the location of the antenna on the map.
	 */
	void setLocation(Point *location);

	/**
	 * Sets the maximum number of simultaneous connections an antenna could handle.
	 * @param maxConnections the maximum number of simultaneous connections an antenna could handle.
	 */
	void setMaxConnections(unsigned long maxConnections);

	/**
	 * Sets the MobileOperator object that owns this antenna.
	 * @param mno a pointer to a MobileOperator object that owns this antenna.
	 */
	void setMno(MobileOperator *mno);

	/**
	 * Sets the network type of the antenna: NetworkType::_3G or NetworkType::_4G.
	 * @param networkType the network type of the antenna: NetworkType::_3G or NetworkType::_4G.
	 */
	void setNetworkType(NetworkType networkType);

	/**
	 * Sets the path loss exponent (the attenuation factor of the signal). This is a feature of the surrounding
	 * environment of the antenna.
	 * @param ple the path loss exponent (the attenuation factor of the signal). This is a feature of the surrounding
	 * environment of the antenna.
	 */
	void setPle(double ple);

	/**
	 * Sets the power of the antenna measured in Watts.
	 * @param power the power of the antenna measured in Watts.
	 */
	void setPower(double power);

	/**
	 * Sets the minimum value of the signal dominance that can be used to connect a mobile device.
	 * @param qmin the minimum value of the signal dominance that can be used to connect a mobile device.
	 */
	void setQmin(double qmin);

	/**
	 * sets the Smid parameter of the signal propagation model. For details see
	 *
	 * Salgado, D., Sanguiao, L., Oancea, B. et al. An end-to-end statistical process with mobile network data for official statistics.
	 * EPJ Data Science, 10, 20 (2021). https://doi.org/10.1140/epjds/s13688-021-00275-w.
	 *
	 * @param Smid the Smid parameter of the signal propagation model.
	 */
	void setSmid(double Smid);

	/**
	 * Sets the minimum value of the signal strength that can be used to connect a mobile device.
	 * @param Smin the minimum value of the signal strength that can be used to connect a mobile device.
	 */
	void setSmin(double Smin);

	/**
	 * sets the SSteep parameter of the signal propagation model. For details see
	 *
	 * Salgado, D., Sanguiao, L., Oancea, B. et al. An end-to-end statistical process with mobile network data for official statistics.
	 * EPJ Data Science, 10, 20 (2021). https://doi.org/10.1140/epjds/s13688-021-00275-w.
	 *
	 * @param SSteep the SSteep parameter of the signal propagation model.
	 */
	void setSSteep(double SSteep);

	/**
	 * Sets the tilt angle of the antenna.
	 * @param tilt the tilt angle of the antenna.
	 */
	void setTilt(double tilt);

	/**
	 * Sets the type of the antenna: AntennaType::DIRECTIONAL or AntennaType::OMNIDIRECTIONAL.
	 * @param type the type of the antenna: AntennaType::DIRECTIONAL or AntennaType::OMNIDIRECTIONAL.
	 */
	void setType(AntennaType type);


private:
	double m_ple;
	double m_power;
	unsigned long m_maxConnections;
	double m_Smid;
	double m_SSteep;
	AntennaType m_type;
	double m_height;
	double m_tilt;
	double m_beam_V;
	double m_beam_H;
	double m_azim_dB_Back;
	double m_elev_dB_Back;
	double m_direction;
	double m_Smin;
	double m_Qmin;
	NetworkType m_networkType;
	Point* m_location;
	MobileOperator *m_MNO;


};

#endif /* INCLUDE_PARSERS_ANTENNACONFIGURATION_H_ */
