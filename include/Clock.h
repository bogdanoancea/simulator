#ifndef CLOCK_H
#define CLOCK_H


class Clock {
    public:
        /** Default constructor */
        Clock();

        /** Constructor */
        Clock(unsigned int init, unsigned int inc);

        /** Default destructor */
        virtual ~Clock();


        void tick();
		unsigned int getCurrentTime() const;
		void setCurrentTime(unsigned int currentTime);
		unsigned int getIncrement() const;
		void setIncrement(unsigned int increment);
		unsigned int getInitialTime() const;
		void setInitialTime(unsigned int initialTime);

    private:
        unsigned int m_initialTime;
        unsigned int m_currentTime;
        unsigned int m_increment;
};

#endif // CLOCK_H
