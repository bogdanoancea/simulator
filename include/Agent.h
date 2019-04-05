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

#include <Agent.h>
#include <World.h>
class Agent {
	public:
		/** Default constructor */
		Agent(World* w);

		/** Default destructor */
		virtual ~Agent();

		bool operator==(const Agent& a);
		const World* getWorld() const;
		void setWorld(World* world);

	private:
		World* m_world;
};

#endif // AGENT_H
