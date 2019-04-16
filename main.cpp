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
#include <unordered_set>

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

	auto itr = c->getAgentListByType(typeid(Person).name());
	vector<long> persons;
	for (auto it = itr.first; it != itr.second; it++) {
		Person* p = dynamic_cast<Person*>(it->second);
		persons.push_back(p->getId());
		cout << setw(10) << p->getId() << setw(15) << p->getLocation().getCoordinate()->x << setw(15) << p->getLocation().getCoordinate()->y
				<< setw(15) << setw(15) << p->getAge() << endl;
	}

	cout << setw(10) << "Antenna ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << " Power " << setw(25) << "Max Connections"
			<< setw(20) << "Attenuation Factor" << endl;

	int noa = 0;
	auto itr2 = c->getAgentListByType(typeid(Antenna).name());
	for (auto it = itr2.first; it != itr2.second; it++) {
		Antenna* a = dynamic_cast<Antenna*>(it->second);
		noa++;
		cout << setw(10) << a->getId() << setw(15) << a->getLocation().getCoordinate()->x << setw(15) << a->getLocation().getCoordinate()->y
				<< setw(15) << a->getPower() << setw(15) << a->getMaxConnections() << setw(15) << a->getAttenuationFactor() << endl;

	}
	cout << "no of antennas " << noa << endl;

	cout << setw(10) << "Mobile Phone ID" << setw(15) << " Owner id " << endl;
	auto itr3 = c->getAgentListByType(typeid(MobilePhone).name());

	int nom = 0;
	for (auto it = itr3.first; it != itr3.second; it++) {
		MobilePhone* a = dynamic_cast<MobilePhone*>(it->second);
		nom++;
		//if (a != nullptr)
		cout << setw(15) << a->getId() << setw(15) << a->getIdHolder() << endl;

	}
	cout << " no of mobile phones : " << nom << endl;
	cout << "... now we give mobile phones to persons... " << endl;
	// randomly select numMobilePhones people
	unordered_set<int> indices;
	while (indices.size() < numMobilePhones) {
		int num = rand() % numPersons;
		indices.insert(num);
	}
	vector<int> pindices;
	std::move(indices.begin(), indices.end(), back_inserter(pindices));
	//unordered_set<int>::iterator iter = indices.begin();
	int i = 0;
	for (auto it = itr3.first; it != itr3.second; it++) {
		MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
		//Person* p = dynamic_cast<Person*>(it->second);
		m->setIdHolder(persons[pindices[i]]);
		//cout << "scot persoana cu indexul " << *iter << endl;
		//std::advance(iter, 1);
		//iter++;

		//p->addDevice(typeid(MobilePhone), persons[indices[i]]);
		i++;
	}


	cout << "Again, the mobiles phones but this time with assigned owners... " << endl;
	cout << setw(10) << "Mobile Phone ID" << setw(15) << " Owner id " << endl;
	for (auto it = itr3.first; it != itr3.second; it++) {
		MobilePhone* a = dynamic_cast<MobilePhone*>(it->second);
		cout << setw(15) << a->getId() << setw(15) << a->getIdHolder() << endl;

	}
	w.runSimulation();

	return 0;
}
