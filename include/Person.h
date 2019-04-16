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

#include <MovableAgent.h>
#include <geos/geom/Point.h>
#include <string>
#include <unordered_map>

using namespace geos;
using namespace geos::geom;

/*
 *
 */
class Person: public MovableAgent {
	public:
		Person(Map* m, long id, Point* initPosition, int age);
		virtual ~Person();

		virtual string getName() { return "Person";}

		int getAge() const;
		void setAge(int age);

		const Point& getPosition() const;
		void setPosition(const Point& position);

		void addDevice(string type, long id) {
			m_idDevices.insert(std::pair<string, long>(type, id));
		}

	private:
		int m_age;
		unordered_multimap<string, long> m_idDevices;

};

#endif /* PERSON_H_ */
