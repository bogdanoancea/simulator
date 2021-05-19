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
 * CellIDTAEventConfig.h
 *
 *  Created on: Oct 21, 2020
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef INCLUDE_EVENTS_CELLIDTAEVENTCONFIG_H_
#define INCLUDE_EVENTS_CELLIDTAEVENTCONFIG_H_

#include <events/CellIDEventConfig.h>

/**
 * This class encapsulated the parameters needed by the factory class to build a  \code{CEllIDTAEvent} object.
 */
class CellIDTAEventConfig: public CellIDEventConfig {

public:
	/**
	 * Constructor of the class. Builds a \code{CellIDTAEventConfig} object.
	 * @param t the timestamp of the event. This is the time measured by the simulation clock.
	 * @param cellID the id of the \code{Antenna} object which generated the event.
	 * @param code - the event code. It can take one of the following values:
	 * \li EventCode::ATTACH_DEVICE
	 * \li EventCode::DETACH_DEVICE
	 * \li EventCode::ALREADY_ATTACHED_DEVICE
	 * \li EventCode::IN_RANGE_NOT_ATTACHED_DEVICE
	 * @param deviceID the id of the mobile device which generated the event.
	 * @param netType the network type (a parameter of the antenna). It can take one of the following values:
	 * \li NetworkType::_3G
	 * \li NetworkType::_4G
	 * @param ta the timing advance value. For an explanation of this variable see \link{CellIDTAEvent}.
	 */
	CellIDTAEventConfig(unsigned long t, unsigned long cellID, EventCode code, unsigned long deviceID, NetworkType netType, unsigned int ta);

	/**
	 * Returns the timing advance value.
	 * @return the timing advance value. For an explanation of this variable see \link{CellIDTAEvent}.
	 */
	const unsigned int getTA();

	/**
	 * Dafault destructor.
	 */
	virtual ~CellIDTAEventConfig();

private:
	unsigned int m_ta;
};

#endif /* INCLUDE_EVENTS_CELLIDTAEVENTCONFIG_H_ */
