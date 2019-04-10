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

		Agent(World& w, long id);

		/** Default destructor */
		virtual ~Agent();

		bool operator==(const Agent& a);

		World& getWorld() const;
		void setWorld(World& world);

		long getId() const {
			return m_id;
		}

		void setId(long id) {
			m_id = id;
		}

	private:
		World& m_world;
		long m_id;
};

#endif // AGENT_H
