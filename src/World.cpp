#include "World.h"
#include <iostream>

using namespace std;
//ctor
World::World(){}

//dtor
World::~World() {}

void World::runSimulation() {
    cout << "Hello from our mobile phone simulator!" << endl;

}

unsigned int World::getCurrentTime() {
    return m_clock.GetCurrentTime();
}
