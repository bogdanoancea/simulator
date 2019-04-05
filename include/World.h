#ifndef WORLD_H
#define WORLD_H

#include "AgentsCollection.h"
#include "Map.h"
#include "Clock.h"


using namespace std;


class World {
    public:
        /** Default constructor */
        World();

        /** Default destructor */
        virtual ~World();

        void runSimulation();

        void dumpState();

        unsigned int getCurrentTime();



    private:
        AgentsCollection m_agents;
        Map m_map;
        Clock m_clock;

};

#endif // WORLD_H
