#include <Agent.h>
#include <AgentsCollection.h>
#include <Antenna.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/Point.h>
#include <geos/io/WKTWriter.h>
#include <Map.h>
#include <MobilePhone.h>
#include <Person.h>
#include <World.h>
#include <iomanip>
#include <unordered_map>
#include <iostream>
#include <string>
#include <typeinfo>


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

	cout << setw(10) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Age " << endl;

	unordered_multimap<string, Agent*>::iterator it = c->getAgentListByType(typeid(Person).name());
	for (; it != c->end(); it++) {
		Person* p = dynamic_cast<Person*>(it->second);
		if (p != nullptr)
			cout << setw(10) << p->getId() << setw(15) << p->getLocation().getCoordinate()->x << setw(15)
					<< p->getLocation().getCoordinate()->y << setw(15) << setw(15) << p->getAge() << endl;
	}

	cout << setw(10) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(25) << "Max Connections"
			<< setw(20) << "Attenuation Factor" << endl;

	int noa = 0;
	unordered_multimap<string, Agent*>::iterator it2 = c->getAgentListByType(typeid(Antenna).name());
	for (; it2 != c->end(); it2++) {
		Antenna* a = dynamic_cast<Antenna*>(it2->second);
		noa++;
		if (a != nullptr)
			cout << setw(10) << a->getId() << setw(15) << a->getLocation().getCoordinate()->x << setw(15)
					<< a->getLocation().getCoordinate()->y << setw(15) << a->getPower() << setw(15) << a->getMaxConnections() << setw(15)
					<< a->getAttenuationFactor() << endl;

	}
	cout << "no of antennas " << noa << endl;



	cout << setw(10) << "Mobile Phone ID" << setw(15) << " Owner id " << endl;
	unordered_multimap<string, Agent*>::iterator it3 = c->getAgentListByType(typeid(MobilePhone).name());

	int nom = 0;
	for (; it3 != c->end(); it3++) {
		MobilePhone* a = dynamic_cast<MobilePhone*>(it3->second);
		nom++;
		if (a != nullptr)
			cout << setw(15) << a->getId() << setw(15) << a->getIdHolder() << endl;

	}
	cout << " no of mobile phones : " << nom << endl;
	cout << "... now we give mobile phones to persons... " << endl;
	// randomly select numMobilePhones people
	it = c->getAgentListByType(typeid(Person).name());
	for (; it != c->end(); it++) {
		Person* p = dynamic_cast<Person*>(it->second);
		if (p != nullptr)
			p->addPhone(-1);
	}


	w.runSimulation();

	return 0;
}
