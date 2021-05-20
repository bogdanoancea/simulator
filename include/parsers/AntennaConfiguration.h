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
 * A class that contains all the technical parameters needed to build an Antenna object.
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
	 * Returns the
	 * @return
	 */
	double getS0() const;

	/**
	 *
	 * @return
	 */
	double getSdAzim() const;

	/**
	 *
	 * @return
	 */
	double getSdElev() const;

	/**
	 *
	 * @return
	 */
	double getSmid() const;

	/**
	 *
	 * @return
	 */
	double getSmin() const;

	/**
	 *
	 * @return
	 */
	double getSSteep() const;

	/**
	 *
	 * @return
	 */
	double getTilt() const;

	/**
	 *
	 * @return
	 */
	AntennaType getType() const;

	/**
	 *
	 * @param mAzimDBBack
	 */
	void setAzimDBBack(double mAzimDBBack);

	/**
	 *
	 * @param mBeamH
	 */
	void setBeamH(double mBeamH);

	/**
	 *
	 * @param mBeamV
	 */
	void setBeamV(double mBeamV);

	/**
	 *
	 * @param mDirection
	 */
	void setDirection(double mDirection);

	/**
	 *
	 * @param mElevDBBack
	 */
	void setElevDBBack(double mElevDBBack);

	/**
	 *
	 * @param mHeight
	 */
	void setHeight(double mHeight);

	/**
	 *
	 * @param mLocation
	 */
	void setLocation(Point *mLocation);

	/**
	 *
	 * @param mMaxConnections
	 */
	void setMaxConnections(unsigned long mMaxConnections);

	/**
	 *
	 * @param mMno
	 */
	void setMno( MobileOperator *mMno);

	/**
	 *
	 * @param mNetworkType
	 */
	void setNetworkType(NetworkType mNetworkType);

	/**
	 *
	 * @param mPle
	 */
	void setPle(double mPle);

	/**
	 *
	 * @param mPower
	 */
	void setPower(double mPower);

	/**
	 *
	 * @param mQmin
	 */
	void setQmin(double mQmin);

	/**
	 *
	 * @param mSdAzim
	 */
	void setSdAzim(double mSdAzim);

	/**
	 *
	 * @param mSdElev
	 */
	void setSdElev(double mSdElev);

	/**
	 *
	 * @param mSmid
	 */
	void setSmid(double mSmid);

	/**
	 *
	 * @param mSmin
	 */
	void setSmin(double mSmin);

	/**
	 *
	 * @param mSSteep
	 */
	void setSSteep(double mSSteep);

	/**
	 *
	 * @param mTilt
	 */
	void setTilt(double mTilt);

	/**
	 *
	 * @param mType
	 */
	void setType(AntennaType mType);

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
