/*
 * Data simulator for mobile phone network events
 *
 * EventType.h
 *
 *  Created on: Apr 24, 2019
 *      Author: Bogdan Oancea
 */

#ifndef EVENTTYPE_H_
#define EVENTTYPE_H_

/*
 *
 */
enum class EventType {
	ATTACH_DEVICE, DETACH_DEVICE, ALREADY_ATTACHED_DEVICE, IN_RANGE_NOT_ATTACHED_DEVICE
};

#endif /* EVENTTYPE_H_ */
