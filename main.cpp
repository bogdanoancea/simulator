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
#include <Utils.h>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace utils;


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

	utils::printPersonHeader();
	auto itr = c->getAgentListByType(typeid(Person).name());
	vector<Person*> persons;
	for (auto it = itr.first; it != itr.second; it++) {
		Person* p = dynamic_cast<Person*>(it->second);
		persons.push_back(p);
		cout << p->toString() << endl;
	}

	utils::printAntennaHeader();
	auto itr2 = c->getAgentListByType(typeid(Antenna).name());
	for (auto it = itr2.first; it != itr2.second; it++) {
		Antenna* a = dynamic_cast<Antenna*>(it->second);
		cout << a->toString() << endl;
	}

	utils::printPhoneHeader();
	auto itr3 = c->getAgentListByType(typeid(MobilePhone).name());
	for (auto it = itr3.first; it != itr3.second; it++) {
		MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
		cout << m->toString() << endl;
	}

	cout << "... now we give mobile phones to persons... " << endl;
	// randomly select numMobilePhones people
	unordered_set<int> indices;
	while (indices.size() < numMobilePhones) {
		int num = rand() % numPersons;
		indices.insert(num);
	}
	vector<int> pindices;
	std::move(indices.begin(), indices.end(), back_inserter(pindices));
	int i = 0;
	for (auto it = itr3.first; it != itr3.second; it++) {
		MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
		Person* p = persons[pindices[i]];
		m->setHolder(p);
		p->addDevice(typeid(MobilePhone).name(), m);
		i++;
	}

	cout << "Again the mobile phones but this time with assigned owners... " << endl;
	utils::printPhoneHeader();
	for (auto it = itr3.first; it != itr3.second; it++) {
		MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
		cout << m->toString() << endl;
	}

	cout << "Again the persons now with mobile phones... " << endl;
	utils::printPersonHeader();
	itr = c->getAgentListByType(typeid(Person).name());
	for (auto it = itr.first; it != itr.second; it++) {
		Person* p = dynamic_cast<Person*>(it->second);
		cout << p->toString() << endl;
	}

	w.runSimulation();

	return (0);
}
