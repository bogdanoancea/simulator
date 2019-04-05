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

class Agent {
	public:
		/** Default constructor */
		Agent();

		/** Default destructor */
		virtual ~Agent();

		bool operator==(const Agent& a);
};

#endif // AGENT_H
