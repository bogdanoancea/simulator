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

#include "Map.h"


class Agent {
	public:

		Agent(Map* m, long id);

		/** Default destructor */
		virtual ~Agent();

		bool operator==(const Agent& a);

		Map* getMap() const;
		void setMap(Map* map);

		long getId() const {
			return m_id;
		}

		void setId(long id) {
			m_id = id;
		}

	private:
		Map* m_map;
		long m_id;
};

#endif // AGENT_H
