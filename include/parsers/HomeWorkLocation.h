/*
 * HomeWorkLocation.h
 *
 *  Created on: Mar 11, 2021
 *      Author: bogdan
 */

#ifndef INCLUDE_HOMEWORKLOCATION_H_
#define INCLUDE_HOMEWORKLOCATION_H_

#include <string>

using namespace std;

class HomeWorkLocation {
public:
	HomeWorkLocation(double x, double y, double z, double sdx, double sdy, double sdz);
	HomeWorkLocation() = delete;
	virtual ~HomeWorkLocation();
	string toString();
	double m_x, m_y, m_z;
	double m_sdx, m_sdy, m_sdz;
};

#endif /* INCLUDE_HOMEWORKLOCATION_H_ */
