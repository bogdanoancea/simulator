#ifndef AGENT_H
#define AGENT_H

#include <Agent.h>


class Agent
{
    public:
        /** Default constructor */
        Agent();
        /** Default destructor */
        virtual ~Agent();

        bool operator==(const Agent& a);
    protected:

    private:
};

#endif // AGENT_H
