/*
 * AntennaConfigurationuration.cpp
 *
 *  Created on: Apr 23, 2021
 *      Author: bogdan
 */

#include <parsers/AntennaConfiguration.h>
#include <Constants.h>

AntennaConfiguration::AntennaConfiguration() {
	m_ple = Constants::ATT_FACTOR;
	m_power = Constants::ANTENNA_POWER;
	m_maxConnections = Constants::ANTENNA_MAX_CONNECTIONS;
	m_Smid = Constants::ANTENNA_S_MID;
	m_SSteep = Constants::ANTENNA_S_STEEP;
	m_type = AntennaType::OMNIDIRECTIONAL;
	m_height = Constants::ANTENNA_HEIGHT;
	m_tilt = Constants::ANTENNA_TILT;
	m_beam_V = Constants::ANTENNA_BEAM_V;
	m_beam_H = Constants::ANTENNA_BEAM_H;
	m_azim_dB_Back = Constants::ANTENNA_AZIM_DB_BACK;
	m_elev_dB_Back = Constants::ANTENNA_ELEV_DB_BACK;
	m_direction = Constants::ANTENNA_DIRECTION;
	m_Smin = Constants::ANTENNA_SMIN;
	m_Qmin = Constants::ANTENNA_QMIN;
	m_networkType = Constants::NETWORK_TYPE;
	m_location = nullptr;
	m_MNO = nullptr;

}

AntennaConfiguration::~AntennaConfiguration() {
	// TODO Auto-generated destructor stub
}

double AntennaConfiguration::getAzimDBBack() const {
	return m_azim_dB_Back;
}

double AntennaConfiguration::getBeamH() const {
	return m_beam_H;
}

double AntennaConfiguration::getBeamV() const {
	return m_beam_V;
}

double AntennaConfiguration::getDirection() const {
	return m_direction;
}

double AntennaConfiguration::getElevDBBack() const {
	return m_elev_dB_Back;
}

double AntennaConfiguration::getHeight() const {
	return m_height;
}

Point* AntennaConfiguration::getLocation() const {
	return m_location;
}

unsigned long AntennaConfiguration::getMaxConnections() const {
	return m_maxConnections;
}

MobileOperator* AntennaConfiguration::getMno() const {
	return m_MNO;
}


NetworkType AntennaConfiguration::getNetworkType() const {
	return m_networkType;
}

double AntennaConfiguration::getPle() const {
	return m_ple;
}

double AntennaConfiguration::getPower() const {
	return m_power;
}

double AntennaConfiguration::getQmin() const {
	return m_Qmin;
}

double AntennaConfiguration::getSmid() const {
	return m_Smid;
}


double AntennaConfiguration::getSmin() const {
	return m_Smin;
}


double AntennaConfiguration::getSSteep() const {
	return m_SSteep;
}


double AntennaConfiguration::getTilt() const {
	return m_tilt;
}

AntennaType AntennaConfiguration::getType() const {
	return m_type;
}

void AntennaConfiguration::setAzimDBBack(double azimDBBack) {
	m_azim_dB_Back = azimDBBack;
}

void AntennaConfiguration::setBeamH(double beamH) {
	m_beam_H = beamH;
}

void AntennaConfiguration::setBeamV(double beamV) {
	m_beam_V = beamV;
}

void AntennaConfiguration::setDirection(double direction) {
	m_direction = direction;
}

void AntennaConfiguration::setElevDBBack(double elevDBBack) {
	m_elev_dB_Back = elevDBBack;
}

void AntennaConfiguration::setHeight(double height) {
	m_height = height;
}

void AntennaConfiguration::setLocation(Point *location) {
	m_location = location;
}

void AntennaConfiguration::setMaxConnections(unsigned long maxConnections) {
	m_maxConnections = maxConnections;
}

void AntennaConfiguration::setMno(MobileOperator *mno) {
	m_MNO = mno;
}

void AntennaConfiguration::setNetworkType(NetworkType networkType) {
	m_networkType = networkType;
}

void AntennaConfiguration::setPle(double ple) {
	m_ple = ple;
}

void AntennaConfiguration::setPower(double power) {
	m_power = power;
}

void AntennaConfiguration::setQmin(double qmin) {
	m_Qmin = qmin;
}

void AntennaConfiguration::setSmid(double Smid) {
	m_Smid = Smid;
}

void AntennaConfiguration::setSmin(double Smin) {
	m_Smin = Smin;
}

void AntennaConfiguration::setSSteep(double SSteep) {
	m_SSteep = SSteep;
}

void AntennaConfiguration::setTilt(double tilt) {
	m_tilt = tilt;
}

void AntennaConfiguration::setType(AntennaType type) {
	m_type = type;
}
