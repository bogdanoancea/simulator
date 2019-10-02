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
 * Clock.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <ctime>
#include <time.h>

using namespace std;
using namespace std::chrono;

/**
 * This is the clock used to synchronize the simulation. All the agents and all other objects involved in a simulation
 * use the same Clock object.
 * The Clock will be initialized with the value of the starting and ending time of the simulation and will keep a current time during
 * the simulation. At each step of the simulation the current time will be increased by an increment. Starting time, ending time, current
 * time and the time increment are only conventional units and they do not depend in any way on the real clock of the computer.
 */
class Clock {
public:
	/**
	 * Default constructor
	 */
	Clock();

	/**
	 * Constructor of the class. It takes the starting and ending time of the simulation as parameters as well as the time increment.
	 * @param start the initial moment when the simulation starts.
	 * @param end the time when simulation ends.
	 * @param incr the time increment. At each step of the simulation the current time is incremented by this quantity.
	 */
	Clock(unsigned long start, unsigned long end, unsigned long incr);

	/**
	 * Default destructor
	 */
	virtual ~Clock();

	/**
	 * increments the current time.
	 * @return the current time after incrementation.
	 */
	unsigned long tick();

	/**
	 * @return the current time of the simulator.
	 */
	unsigned long getCurrentTime() const;

	/**
	 * Sets the current time of the simulator.
	 * @param currentTime the value of the current time to be set.
	 */
	void setCurrentTime(unsigned long currentTime);

	/**
	 * @return the time increment used in simulation.
	 */
	unsigned long getIncrement() const;

	/**
	 * Sets the time increment to be used in a simulation.
	 * @param increment the value of the time increment.
	 */
	void setIncrement(unsigned long increment);

	/**
	 *@return the starting time of the simulation.
	 */
	unsigned long getInitialTime() const;

	/**
	 * Sets the starting time of the simulation.
	 * @param initialTime the value of the starting time of the simulation.
	 */
	void setInitialTime(unsigned long initialTime);

	/**
	 *
	 * @return the real time read from the computer clock. It is used only to register the
	 * exact date and time of a simulation.
	 */
	time_t realTime();

	/**
	 *@return the ending time of the simulation.
	 */
	unsigned long getFinalTime() const;

	/**
	 * Sets the ending time of a simulation.
	 * @param finalTime the value of the ending time of a simulation.
	 */
	void setFinalTime(unsigned long finalTime);

	/**
	 * Resets the current time and makes it equal to the starting time such that a new simulation can begin.
	 */
	void reset();

private:
	unsigned long m_initialTime;
	unsigned long m_currentTime;
	unsigned long m_increment;
	unsigned long m_finalTime;
};

#endif // CLOCK_H
