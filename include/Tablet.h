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
		Tablet(World& w, Point& initPosition, long tabletId);
		virtual ~Tablet();
		long getTabletId() const;
		void settabletId(long phoneId);

	private:
		long m_tabletId;

};

#endif /* TABLET_H_ */
