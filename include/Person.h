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
		enum Gender {
			MALE, FEMALE
		};
		enum AgeDistributions {
			NORMAL, UNIFORM
		};

		explicit Person(Map* m, long id, Point* initPosition, Clock* clock, double initSpeed, int age, Gender gender);

		virtual ~Person();

		string getName() override {
			return ("Person");
		}

		string toString() override;

		string dumpDevices();
		bool hasDevices();

		int getAge() const;
		void setAge(int age);


		Point* move(MovementType mvType) override;
		virtual void setLocation(Point* pt) override;

		void addDevice(string type, Agent* agent) {
			m_idDevices.insert(std::pair<string, Agent*>(type, agent));
		}

	Gender getGender() const;

	private:
		int m_age;
		Gender m_gender;
		unordered_multimap<string, Agent*> m_idDevices;
		void randomWalk();

};

#endif /* PERSON_H_ */
