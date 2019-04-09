/*
 * Data simulator for mobile phone network events
 *
 * Agent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef AGENT_H
#define AGENT_H

#include "World.h"

class Agent {
	public:
		/** Default constructor */
		Agent();

		Agent(World* world);

		/** Default destructor */
		virtual ~Agent();

		bool operator==(const Agent& a);

		World* getWorld() const;
		void setWorld(World* world);

	private:
		World* m_world;
};

#endif // AGENT_H
