/*
 * LocatableAgent.h
 *
 *  Created on: Apr 4, 2019
 *      Author: bogdan
 */

#ifndef LOCATABLEAGENT_H_
#define LOCATABLEAGENT_H_

#include <Agent.h>
#include <geos/geom/Point.h>

using namespace geos;
using namespace geos::geom;

class LocatableAgent: public Agent {
	public:
		LocatableAgent(Point initLocation);

		virtual ~LocatableAgent();
		const Point& getLocation() const;
		void setLocation(const Point& location);

	private:
		Point m_location;
};

#endif /* LOCATABLEAGENT_H_ */
