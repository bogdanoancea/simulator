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


#include <Map.h>
#include <Clock.h>
#include <string>

class Agent {
	public:

		Agent(Map* m, long id, Clock* clock);

		/** Default destructor */
		virtual ~Agent();

		bool operator==(const Agent& a);

		virtual string getName() = 0;
		virtual string toString() = 0;

		Map* getMap() const;
		void setMap(Map* map);

		Clock* getClock() const;

		long getId() const {
			return m_id;
		}

		void setId(long id) {
			m_id = id;
		}

	private:
		Map* m_map;
		long m_id;
		Clock* m_clock;
};

#endif // AGENT_H
