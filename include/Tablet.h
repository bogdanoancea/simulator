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
#include <MovementType.h>
/*
 *
 */
class Tablet: public HoldableAgent {
	public:
		explicit Tablet(Map* m, long id, Point* initPosition, Clock* clock);
		virtual ~Tablet();

		const string getName() override {
			return ("Tablet");
		}

		string toString() const override;

		Point* move(MovementType type) override {
			return getLocation();
		}

		bool tryConnect() override;

	private:


};

#endif /* TABLET_H_ */
