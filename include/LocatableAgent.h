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
#include <Clock.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

class LocatableAgent: public Agent {
	public:
		explicit LocatableAgent(Map* m, long id, Point* initLocation);

		virtual ~LocatableAgent();

		string getName() override {
			return "LocatableAgent";
		}

		string toString() override;

		Point* getLocation() const;
		void setLocation(Point* location);

		string dumpLocation(Clock* clock);

	private:
		Point* m_location;
};

#endif /* LOCATABLEAGENT_H_ */
