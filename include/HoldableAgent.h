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

		virtual ~HoldableAgent();

		Agent* getHolder() const;
		void setHolder(Agent* holder);

		string getName() override {
			return "HoldableAgent";
		}

		string toString() override;
		virtual bool tryConnect() = 0;
		virtual bool isConnected() const;
		vector<Antenna*> getAntennas() const;
		//virtual void setConnected(bool c);

	private:
		Agent* m_holder;
		//bool m_isConnected;
		vector<Antenna*> m_antennas;
};

#endif /* HOLDABLEAGENT_H_ */
