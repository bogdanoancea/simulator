#ifndef AGENTSCOLLECTION_H
#define AGENTSCOLLECTION_H


#include "World.h"
#include "Agent.h"
#include<vector>



class AgentsCollection
{
    public:
        /** Default constructor */
        AgentsCollection();

        /** Default destructor */
        virtual ~AgentsCollection();

    protected:

    private:
        World world;
        vector<Agent> agents;
};

#endif // AGENTSCOLLECTION_H
