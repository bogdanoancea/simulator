/*
 * Person.h
 *
 *  Created on: Apr 5, 2019
 *      Author: bogdan
 */

#ifndef PERSON_H_
#define PERSON_H_

#include <geos/geom/Point.h>
#include <MovableAgent.h>

using namespace geos;
using namespace geos::geom;


/*
 *
 */
class Person: public MovableAgent {
	public:
		Person();
		virtual ~Person();

		int getAge() const;
		void setAge(int age);
		long getId() const;
		void setId(long id);
		const Point& getPosition() const;
		void setPosition(const Point& position);

	private:
		long m_id;
		int m_age;
		Point m_position;



};

#endif /* PERSON_H_ */
