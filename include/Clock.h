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

using namespace std;
using namespace std::chrono;

class Clock {
	public:
		/** Default constructor */
		Clock();

		/** Constructor */
		Clock(unsigned long start, unsigned long end, unsigned long incr);

		/** Default destructor */
		virtual ~Clock();

		unsigned long tick();

		unsigned long getCurrentTime() const;
		void setCurrentTime(unsigned long currentTime);

		unsigned long getIncrement() const;
		void setIncrement(unsigned long increment);

		unsigned long getInitialTime() const;
		void setInitialTime(unsigned long initialTime);

		time_t realTime();
		unsigned long getFinalTime() const;
		void setFinalTime(unsigned long finalTime);
		void reset();

	private:
		unsigned long m_initialTime;
		unsigned long m_currentTime;
		unsigned long m_increment;
		unsigned long m_finalTime;
};

#endif // CLOCK_H
