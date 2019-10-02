/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
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
 * ATTACH_DEVICE - means that an antenna connects a new mobile device.
 * DETACH_DEVICE - means that an antenna disconnects a mobile device, i.e. this mobile device is no longer connected to that antenna.
 * ALREADY_ATTACHED_DEVICE - means that an antenna is connected with a mobile device and that mobile was connected to the same antenna
 * in the previous time step too.
 * IN_RANGE_NOT_ATTACHED_DEVICE - means that a mobile device tried to connect to an antenna, the antenna provided enough signal power/quality
 * but the connection was not successful (for example antenna is at its maximum capacity and cannot connect any new devices).
 */
enum class EventType {
	ATTACH_DEVICE, DETACH_DEVICE, ALREADY_ATTACHED_DEVICE, IN_RANGE_NOT_ATTACHED_DEVICE
};

#endif /* EVENTTYPE_H_ */
