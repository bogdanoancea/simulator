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

#include "ImmovableAgent.h"
#include <geos/geom/Polygon.h>


using namespace geos;
using namespace geos::geom;

/*
 *
 */
class Antenna: public ImmovableAgent {
	public:
		explicit Antenna(Map* m, long id, Point* initPosition, double attenuationFactor, double power, int maxConnections);
		virtual ~Antenna();

		string getName() override {
			return "Antenna";
		}

		string toString() override;

		double getAttenuationFactor() const;
		void setAttenuationFactor(double attenuationFactor);

		Polygon* getCell() const;
		void setCell(const Polygon& cell);

		double getPower() const;
		void setPower(double power);

		int getMaxConnections() const;
		void setMaxConnections(int maxConnections);

	private:
		double m_power;
		double m_attenuationFactor;
		Polygon* m_cell;
		int m_maxConnections;


};

#endif /* ANTENNA_H_ */
