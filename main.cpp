#include <Agent.h>
#include <AgentsCollection.h>
#include <Antenna.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Point.h>
#include <geos/io/WKTWriter.h>
#include <Map.h>
#include <Person.h>
#include <World.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <typeinfo>
#include <unordered_map>

using namespace std;
using namespace geos;
using namespace geos::geom;

int main() {
	cout << "Hello from our mobile phone network simulator!" << endl;
	cout << "Now we are building the world!" << endl;
	Map* map = new Map(0, 0, 10, 10);
	geos::io::WKTWriter writter;
	cout << "Our world has a map:" << endl << writter.write(map->getBoundary()) << endl;

	int numPersons = 100;
	int numAntennas = 10;
	cout << "... and it has " << numPersons << " persons and " << numAntennas << " antennas!" << endl;

	World w(map, numPersons, numAntennas);
	AgentsCollection* c = w.getAgents();

	unordered_multimap<string, Agent*>::iterator it = c->getAgentListByType(typeid(Person).name());
	cout << setw(10) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Age " << endl;
//	for (int i = 0; i < numPersons; i++) {
//		Person* p = dynamic_cast<Person*>(c->getAgent(i));
//		cout << setw(10) << p->getId() << setw(15) << p->getLocation().getCoordinate()->x << setw(15)
//				<< p->getLocation().getCoordinate()->y << setw(15) << setw(15) << p->getAge() << endl;
//	}



	 while (it != c->end()) {
		Person* p = dynamic_cast<Person*>(it->second);
		cout << setw(10) << p->getId() << setw(15) << p->getLocation().getCoordinate()->x << setw(15) << p->getLocation().getCoordinate()->y
				<< setw(15) << setw(15) << p->getAge() << endl;
		it++;
	}
	cout << setw(10) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(25) << "Max Connections"
			<< setw(20) << "Attenuation Factor" << endl;

	for (int i = numPersons; i < numPersons + numAntennas; i++) {
		Antenna* a = dynamic_cast<Antenna*>(c->getAgent(i));
		cout << setw(10) << a->getId() << setw(15) << a->getLocation().getCoordinate()->x << setw(15) << a->getLocation().getCoordinate()->y
				<< setw(15) << a->getPower() << setw(15) << a->getMaxConnections() << setw(15) << a->getAttenuationFactor() << endl;
	}

	w.runSimulation();

	return 0;
}
