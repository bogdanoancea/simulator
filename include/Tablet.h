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
		Tablet(Map* m, long id, Point& initPosition);
		virtual ~Tablet();

	private:


};

#endif /* TABLET_H_ */
