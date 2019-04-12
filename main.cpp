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
	double market_share = 0.3;
	int numMobilePhones = numPersons * market_share;
	cout << "... and it has " << numPersons << " persons and " << numAntennas << " antennas" << " and " << numMobilePhones
			<< " mobile phones!" << endl;

	World w(map, numPersons, numAntennas, numMobilePhones);
	AgentsCollection* c = w.getAgents();

	unordered_multimap<string, Agent*>::iterator it = c->getAgentListByType(typeid(Person).name());
	cout << setw(10) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Age " << endl;

	while (it != c->end()) {
		Person* p = dynamic_cast<Person*>(it->second);
		cout << setw(10) << p->getId() << setw(15) << p->getLocation().getCoordinate()->x << setw(15) << p->getLocation().getCoordinate()->y
				<< setw(15) << setw(15) << p->getAge() << endl;
		it++;
	}
	cout << setw(10) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(25) << "Max Connections"
			<< setw(20) << "Attenuation Factor" << endl;

	unordered_multimap<string, Agent*>::iterator it2 = c->getAgentListByType(typeid(Antenna).name());
	while (it2 != c->end()) {
		Antenna* a = dynamic_cast<Antenna*>(it2->second);
		cout << setw(10) << a->getId() << setw(15) << a->getLocation().getCoordinate()->x << setw(15) << a->getLocation().getCoordinate()->y
				<< setw(15) << a->getPower() << setw(15) << a->getMaxConnections() << setw(15) << a->getAttenuationFactor() << endl;
		it2++;
	}

	cout << setw(10) << "Mobile Phone ID" << setw(15) << " Owner id " << endl;
//	unordered_multimap<string, Agent*>::iterator it3 = c->getAgentListByType(typeid(MobilePhone).name());
//
//
//	while (it3 != c->end()) {
//		MobilePhone* a = dynamic_cast<MobilePhone*>(it3->second);
//		cout << setw(15) << a->getId() << setw(15) << a->getIdHolder() << endl;
//		it3++;
//	}
	w.runSimulation();

	return 0;
}
