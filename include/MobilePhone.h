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

#include <HoldableAgent.h>

/*
 *
 */
class MobilePhone: public HoldableAgent {
	public:
		MobilePhone();
		virtual ~MobilePhone();
		long getPhoneId() const;
		void setPhoneId(long phoneId);

	private:
		long m_phoneId;

};

#endif /* MOBILEPHONE_H_ */
