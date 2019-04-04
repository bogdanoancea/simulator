#ifndef CLOCK_H
#define CLOCK_H


class Clock
{
    public:
        /** Default constructor */
        Clock();

        /** Constructor */
        Clock(unsigned int init, unsigned int inc);

        /** Default destructor */
        virtual ~Clock();

        /** Access m_currentTime
         * \return The current value of m_currentTime
         */
        unsigned int GetCurrentTime() { return m_currentTime; }

        /** Set m_currentTime
         * \param val New value to set
         */
        void SetCurrentTime(unsigned int val) { m_currentTime = val; }

        unsigned int GetIncrement() { return m_increment; }
        void SetIncrement(unsigned int val) { m_increment = val; }

        unsigned int GetInitialTime() { return m_initialTime; }
        void SetInitialTime(unsigned int val) { m_initialTime = val; }

        void tick();



    private:
        unsigned int m_initialTime;
        unsigned int m_currentTime;
        unsigned int m_increment;
};

#endif // CLOCK_H
