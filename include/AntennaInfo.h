/*
 * Data simulator for mobile phone network events
 *
 * AntennaInfo.h
 *
 *  Created on: May 16, 2019
 *      Author: Bogdan Oancea
 */

#ifndef ANTENNAINFO_H_
#define ANTENNAINFO_H_

/*
 *
 */
class AntennaInfo {
	public:
		AntennaInfo(unsigned long time, unsigned long antennaId, unsigned long event, unsigned long deviceId, double x, double y);
	private:
		unsigned long m_time;
		unsigned long m_antennaId;
		unsigned long m_eventCode;
		unsigned long m_deviceId;
		double m_x;
		double m_y;
};

#endif /* ANTENNAINFO_H_ */
