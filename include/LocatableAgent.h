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
using namespace std;

class LocatableAgent: public Agent {
public:
	explicit LocatableAgent(Map* m, long id, Point* initLocation, Clock* clock);

	virtual ~LocatableAgent();

	string getName() override {
		return "LocatableAgent";
	}

	string toString() override;

	Point* getLocation() const;
	void setLocation(Point* location);

	string dumpLocation(Clock* clock);

	const unique_ptr<Point>& getLocation2() const;
	void setLocation2(const unique_ptr<Point> location);

private:
	unique_ptr<Point> m_location2;
	Point* m_location;
};

#endif /* LOCATABLEAGENT_H_ */
