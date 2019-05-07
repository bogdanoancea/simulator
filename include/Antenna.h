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

using namespace geos;
using namespace geos::geom;

/*
 *
 */
class Antenna: public ImmovableAgent {
	public:
		explicit Antenna(Map* m, long id, Point* initPosition, Clock* clock, double attenuationFactor, double power,
				unsigned long maxConnections, AntennaType type);


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

		unsigned long getMaxConnections() const;
		void setMaxConnections(int maxConnections);

		bool tryRegisterDevice(HoldableAgent* device);
		void attachDevice(HoldableAgent* device);
		void dettachDevice(HoldableAgent* device);

		AntennaType getType() const;

		void setType(AntennaType type);

	private:

		bool alreadyRegistered(HoldableAgent * ag);
		void registerEvent(HoldableAgent * ag, EventType event, bool verbose);
		unsigned long getNumActiveConections();

		double m_power;
		double m_attenuationFactor;
		Polygon* m_cell;
		unsigned long m_maxConnections;
		//int m_numActiveConnections;
		vector<HoldableAgent*> m_devices;
		AntennaType m_type;
};

#endif /* ANTENNA_H_ */
