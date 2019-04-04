#ifndef AGENTSCOLLECTION_H
#define AGENTSCOLLECTION_H


#include "Agent.h"
#include<vector>

using namespace std;

class AgentsCollection
{
    public:
        /** Default constructor */
        AgentsCollection();

        /** Default destructor */
        virtual ~AgentsCollection();
        unsigned int getCurrentTime();
        void addAgent(Agent);
        void deleteAgent(Agent);
        void performAction();

    protected:

    private:
        vector<Agent> agents;
};

#endif // AGENTSCOLLECTION_H
