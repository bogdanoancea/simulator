/*
 * Data simulator for mobile phone network events
 *
 * MobilePhone.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef MOBILEPHONE_H_
#define MOBILEPHONE_H_

#include <HoldableAgent.h>
#include <Antenna.h>
#include <Clock.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

/*
 *
 */
class MobilePhone: public HoldableAgent {
	public:
		explicit MobilePhone(Map* m, long id, Point* initPosition, Agent* holder, Clock* clock, double powerThreshold, double qualityThreshold, HoldableAgent::CONNECTION_TYPE connType);
		virtual ~MobilePhone();

		const string getName() override {
			return ("MobilePhone");
		}

		string toString() override;

		Point* move(MovementType mvType) override {
			return (this->getLocation());
		}

		bool tryConnect(HoldableAgent::CONNECTION_TYPE type) override;

		double getQualityThreshold() const;
		void setQualityThreshold(double qualityThreshold);
		double getPowerThreshold() const;
		void setPowerThreshold(double powerThreshold);

	private:
		bool tryConnectNaiveAlgorithm(HoldableAgent::CONNECTION_TYPE type);
		double m_powerThreshold;
		double m_qualityThreshold;
		Antenna* m_connectedTo;
		HoldableAgent::CONNECTION_TYPE m_connType;

};

#endif /* MOBILEPHONE_H_ */
