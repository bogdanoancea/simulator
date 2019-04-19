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
		Person(Map* m, long id, Point* initPosition, double initSpeed, int age);
		virtual ~Person();

		string getName() override {
			return "Person";
		}

		string toString() override;

		string dumpDevices();
		bool hasDevices();

		int getAge() const;
		void setAge(int age);

		const Point& getPosition() const;
		void setPosition(const Point& position);

		Point& move(std::mt19937& generator) override;

		void addDevice(string type, Agent* agent) {
			m_idDevices.insert(std::pair<string, Agent*>(type, agent));
		}

	private:
		int m_age;
		unordered_multimap<string, Agent*> m_idDevices;

};

#endif /* PERSON_H_ */
