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

/**
 * This class represents a mobile phone.
 */
class MobilePhone: public HoldableAgent {
public:
	/**
	 * Builds a new object with the parameters provided by the user
	 * @param m a pointer to the map object where the simulation takes place
	 * @param id the id of the mobile phone
	 * @param initPosition the initial location of the phone on the map
	 * @param holder a pointer to the Agent object that owns this mobile phone
	 * @param clock a pointer to the Clock object used in this simulation
	 * @param powerThreshold the minimum power of the field below which the mobile phone cannot connect to an antenna
	 * @param qualityThreshold the minimum quality of the signal below which the mobile phone cannot connect to an antenna
	 * @param connType the type of the connection to an antenna: based on the power of the signal or based on the signal quality. It could
	 * take two values: HoldableAgent::CONNECTION_TYPE::USING_POWER or HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY
	 *
	 */
	explicit MobilePhone(const Map* m, long id, Point* initPosition, Agent* holder, const Clock* clock, double powerThreshold, double qualityThreshold,
			HoldableAgent::CONNECTION_TYPE connType);

	/**
	 * Destructor
	 */
	virtual ~MobilePhone();

	/**
	 * Returns the name of this class
	 * @return the name of this class
	 */
	const string getName() const override;
	/**
	 * Returns a human readable string representation of this class useful to output it to a file or console
	 * @return a human readable string representation of this class
	 */
	const string toString() const override;

	/**
	 * Makes a step on the map according to an algorithm. The direction and the length of the step is determined by the
	 * mvType parameter and by the person object who owns this phone.
	 * @param mvType selects the way people and phone are moving on the map. At this moment only RANDOM_WALK is implemented.
	 * @return the final location after the movement.
	 */
	Point* move(MovementType mvType) override {
		return (this->getLocation());
	}

	/**
	 * This method is called after the phone moves (together with its owner) to a new location. It tries to connect the
	 * mobile phone to an antenna
	 * @param type determines the connection method: using the power of the signal or using the quality of the signal. The allowed values for this
	 * parameter are: HoldableAgent::CONNECTION_TYPE::USING_POWER or HoldableAgent::CONNECTION_TYPE::USING_SIGNAL_QUALITY
	 * @return true if the connection succeeds, false otherwise
	 */
	bool tryConnect() override;


	/**
	 * Returns the minimum value of the signal quality below which the phone cannot use the signal (i.e. the signal is considered noise)
	 * @return the minimum value of the signal quality below which the phone cannot use the signal (i.e. the signal is considered noise)
	 */
	double getQualityThreshold() const;

	/**
	 * Sets the minimum value of the signal quality below which the phone cannot use the signal (i.e. the signal is considered noise)
	 * @param qualityThreshold the minimum value of the signal quality below which the phone cannot use the signal
	 */
	void setQualityThreshold(double qualityThreshold);

	/**
	 * Returns the minimum value of the signal power below which the phone cannot use the signal (i.e. the signal is considered noise)
	 * @return the minimum value of the signal power below which the phone cannot use the signal (i.e. the signal is considered noise)
	 */
	double getPowerThreshold() const;

	/**
	 * Sets the minimum value of the signal power below which the phone cannot use the signal (i.e. the signal is considered noise)
	 * @param qualityThreshold the minimum value of the signal power below which the phone cannot use the signal
	 */
	void setPowerThreshold(double powerThreshold);

private:
	//bool tryConnectNaiveAlgorithm(HoldableAgent::CONNECTION_TYPE type);
	double m_powerThreshold;
	double m_qualityThreshold;
	Antenna* m_connectedTo;
	HoldableAgent::CONNECTION_TYPE m_connType;

};

#endif /* MOBILEPHONE_H_ */
