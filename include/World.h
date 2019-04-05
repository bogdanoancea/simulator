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

		const AgentsCollection& getAgents() const;
		void setAgents(const AgentsCollection& agents);
		const Clock& getClock() const;
		void setClock(const Clock& clock);
		const Map& getMap() const;
		void setMap(const Map& map);

    private:
        AgentsCollection m_agents;
        Map m_map;
        Clock m_clock;

};

#endif // WORLD_H
