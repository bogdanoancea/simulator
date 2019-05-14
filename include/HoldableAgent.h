/*
 * Data simulator for mobile phone network events
 *
 * HoldableAgent.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef HOLDABLEAGENT_H_
#define HOLDABLEAGENT_H_

#include <MovableAgent.h>
#include <Clock.h>
#include <string>

class Antenna;
/*
 *
 */
class HoldableAgent: public MovableAgent {
	public:
		explicit HoldableAgent(Map* m, long id, Point* initPosition, Agent* holder, Clock* clock);
		HoldableAgent(const HoldableAgent &h);
		enum CONNECTION_TYPE {
			USING_POWER, USING_SIGNAL_QUALITY
		};

		virtual ~HoldableAgent();

		Agent* getHolder() const;
		void setHolder(Agent* holder);

		string getName() override {
			return ("HoldableAgent");
		}

		string toString() override;
		virtual bool tryConnect(CONNECTION_TYPE type) = 0;
		virtual bool isConnected() const;
		vector<Antenna*> getAntennas() const;


	private:
		Agent* m_holder;
		vector<Antenna*> m_antennas;
};

#endif /* HOLDABLEAGENT_H_ */
