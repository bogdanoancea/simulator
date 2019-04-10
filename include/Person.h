/*
 * Data simulator for mobile phone network events
 *
 * Person.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */



#ifndef PERSON_H_
#define PERSON_H_

#include <geos/geom/Point.h>
#include "MovableAgent.h"

using namespace geos;
using namespace geos::geom;

/*
 *
 */
class Person: public MovableAgent {
	public:
		Person(Map* m, long id, Point& initPosition, int age);
		virtual ~Person();

		int getAge() const;
		void setAge(int age);

		const Point& getPosition() const;
		void setPosition(const Point& position);

	private:
		int m_age;

};

#endif /* PERSON_H_ */
