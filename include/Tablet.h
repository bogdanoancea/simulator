/*
 * Data simulator for mobile phone network events
 *
 * Tablet.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef TABLET_H_
#define TABLET_H_

#include <HoldableAgent.h>

/*
 *
 */
class Tablet: public HoldableAgent {
	public:
		Tablet(Map* m, long id, Point* initPosition);
		virtual ~Tablet();

		string getName() override {
			return "Tablet";
		}

		string toString() override;
		Point& move(std::mt19937 &generator) override {
			return getLocation();
		}

		bool tryConnect() override;

	private:


};

#endif /* TABLET_H_ */
