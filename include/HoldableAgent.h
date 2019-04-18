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
#include <string>


/*
 *
 */
class HoldableAgent: public MovableAgent {
	public:
		HoldableAgent(Map* m, long id, Point* initPosition, Agent* holder);
		virtual ~HoldableAgent();

		Agent* getHolder() const;
		void setHolder(Agent* holder);

		string getName() override {
			return "HoldableAgent";
		}

		string toString() override;
	private:
		Agent* m_holder;
};

#endif /* HOLDABLEAGENT_H_ */
