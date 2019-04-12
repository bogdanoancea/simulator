/*
 * Data simulator for mobile phone network events
 *
 * LocatableAgent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */

#ifndef LOCATABLEAGENT_H_
#define LOCATABLEAGENT_H_

#include <Agent.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

class LocatableAgent: public Agent {
	public:
		LocatableAgent(Map* m, long id, Point* initLocation);

		virtual ~LocatableAgent();

		virtual string getName() { return "LocatableAgent";}

		Point& getLocation() const;
		void setLocation(Point& location);

	private:
		Point* m_location;
};

#endif /* LOCATABLEAGENT_H_ */
