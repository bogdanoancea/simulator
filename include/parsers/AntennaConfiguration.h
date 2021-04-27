/*
 * AntennaConfiguration.h
 *
 *  Created on: Apr 23, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PARSERS_ANTENNACONFIGURATION_H_
#define INCLUDE_PARSERS_ANTENNACONFIGURATION_H_

#include <AntennaType.h>
#include <NetworkType.h>
#include <agent/MobileOperator.h>
#include <geos/geom/Point.h>



class AntennaConfiguration {
public:
	AntennaConfiguration();
	virtual ~AntennaConfiguration();
	double getAzimDBBack() const;
	double getBeamH() const;
	double getBeamV() const;
	double getDirection() const;
	double getElevDBBack() const;
	double getHeight() const;
	Point* getLocation() const;
	unsigned long getMaxConnections() const;
	MobileOperator* getMno() const;
	NetworkType getNetworkType() const;
	double getPle() const;
	double getPower() const;
	double getQmin() const;
	double getS0() const;
	double getSdAzim() const;
	double getSdElev() const;
	double getSmid() const;
	double getSmin() const;
	double getSSteep() const;
	double getTilt() const;
	AntennaType getType() const;
	void setAzimDBBack(double mAzimDBBack);
	void setBeamH(double mBeamH);
	void setBeamV(double mBeamV);
	void setDirection(double mDirection);
	void setElevDBBack(double mElevDBBack);
	void setHeight(double mHeight);
	void setLocation(Point *mLocation);
	void setMaxConnections(unsigned long mMaxConnections);
	void setMno( MobileOperator *mMno);
	void setNetworkType(NetworkType mNetworkType);
	void setPle(double mPle);
	void setPower(double mPower);
	void setQmin(double mQmin);
	void setSdAzim(double mSdAzim);
	void setSdElev(double mSdElev);
	void setSmid(double mSmid);
	void setSmin(double mSmin);
	void setSSteep(double mSSteep);
	void setTilt(double mTilt);
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
