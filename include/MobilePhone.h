/*
 * Data simulator for mobile phone network events
 *
 * MobilePhone.h
 *
 *  Created on: Apr 5, 2019
 *      Author: Bogdan Oancea
 */

#ifndef MOBILEPHONE_H_
#define MOBILEPHONE_H_

#include "HoldableAgent.h"

/*
 *
 */
class MobilePhone: public HoldableAgent {
	public:
		MobilePhone(Map* m, long id, Point* initPosition, long id_holder);
		virtual ~MobilePhone();

		virtual string getName() { return "MobilePhone";}
	private:


};

#endif /* MOBILEPHONE_H_ */
