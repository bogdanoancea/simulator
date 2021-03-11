/*
 * HomeWorkLocation.h
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_PARSERS_HOMEWORKLOCATION_H_
#define INCLUDE_PARSERS_HOMEWORKLOCATION_H_

class HomeWorkLocation {
public:
	HomeWorkLocation(double x, double y, double z, double sdx, double sdy, double sdz);
	virtual ~HomeWorkLocation();
	double m_x, m_y, m_z;
	double m_sdx, m_sdy, m_sdz;
};

#endif /* INCLUDE_PARSERS_HOMEWORKLOCATION_H_ */
