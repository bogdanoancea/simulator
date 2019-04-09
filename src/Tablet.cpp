/*
 * Data simulator for mobile phone network events
 *
 * Tablet.cpp
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#include <Tablet.h>

Tablet::Tablet(World& w, Point& initPosition, long tabletId) :
		HoldableAgent(w, initPosition, -1), m_tabletId { tabletId } {
// TODO Auto-generated constructor stub

}

Tablet::~Tablet() {
	// TODO Auto-generated destructor stub
}

