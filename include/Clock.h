/*
 * Data simulator for mobile phone network events
 *
 * Clock.h
 *
 *  Created on: Apr 4, 2019
 *      Author: Bogdan Oancea
 */


#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>
#include <ctime>
#include <time.h>

using namespace std::chrono;

class Clock {
	public:
		/** Default constructor */
		Clock();

		/** Constructor */
		Clock(unsigned int init, unsigned int inc);

		/** Default destructor */
		virtual ~Clock();

		unsigned int tick();

		unsigned int getCurrentTime() const;
		void setCurrentTime(unsigned int currentTime);

		unsigned int getIncrement() const;
		void setIncrement(unsigned int increment);

		unsigned int getInitialTime() const;
		void setInitialTime(unsigned int initialTime);

		time_t realTime();

		unsigned int getFinalTime() const {
			return m_finalTime;
		}

		void setFinalTime(unsigned int finalTime) {
			m_finalTime = finalTime;
		}

	private:
		unsigned int m_initialTime;
		unsigned int m_currentTime;
		unsigned int m_increment;
		unsigned int m_finalTime;
};

#endif // CLOCK_H
