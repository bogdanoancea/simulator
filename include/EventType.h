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

/**
 * This class is an enumeration of network events currently registered:
 * ATTACH_DEVICE - means that an antenna connects a new mobile device
 * DETACH_DEVICE - means that an antenna disconnects a mobile device, i.e. this mobile device is no longer connected to that antenna
 * ALREADY_ATTACHED_DEVICE - means that an antenna is connected with a mobile device and that mobile was connected to the same antenna
 * in the previous time step too.
 * IN_RANGE_NOT_ATTACHED_DEVICE - means that a mobile device tried to connect to an antenna, the antenna provided enough signal power/quality
 * but the connection was not successful (for example antenna is at its maximum capacity and cannot connect any new devices.
 */
enum class EventType {
	ATTACH_DEVICE, DETACH_DEVICE, ALREADY_ATTACHED_DEVICE, IN_RANGE_NOT_ATTACHED_DEVICE
};

#endif /* EVENTTYPE_H_ */
