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
		explicit Tablet(Map* m, long id, Point* initPosition);
		virtual ~Tablet();

		string getName() override {
			return "Tablet";
		}

		string toString() override;
		Point* move() override {
			return getLocation();
		}

		bool tryConnect() override;

	private:


};

#endif /* TABLET_H_ */
