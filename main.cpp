#include <AgentsCollection.h>
#include <Antenna.h>
#include <geos/io/WKTWriter.h>
#include <Map.h>
#include <MobilePhone.h>
#include <Person.h>
#include <Utils.h>
#include <World.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <Constants.h>
#include <InputParser.h>
#include <exception>
#include <typeinfo>
#include <stdexcept>

#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace utils;

int main(int argc, char** argv) {

	InputParser parser(argc, argv);
	if (argc == 2 && parser.cmdOptionExists("-h")) {
		cout << "run this program like this: simulator -a <antennasConfigFile.xml> -m <mapFile.wkt>" << endl;
		exit(0);
	}

	const std::string &antennasConfigFileName = parser.getCmdOption("-a");
	const std::string &mapFileName = parser.getCmdOption("-m");
	const std::string &personsFileName = parser.getCmdOption("-p");

	cout << "Hello from our mobile phone network simulator!" << endl;
	cout << "Now we are building the world!" << endl;

	try {
		Map* map;
		if (mapFileName.empty()) {
			throw runtime_error("no map file!");
		}
		else
			map = new Map(mapFileName);

		geos::io::WKTWriter writter;
		cout << "Our world has a map:" << endl << writter.write(map->getBoundary()) << endl;

		//unsigned long numPersons = Constants::NO_PERSONS;
		unsigned long numMobilePhones = Constants::NO_MOBILE_PHONES;

		//cout << "... and it has " << numPersons << " persons and " << numMobilePhones
		//		<< " mobile phones!" << endl;

		if (antennasConfigFileName.empty()) {
			throw runtime_error("no antenna config file!");
		}
		if (personsFileName.empty()) {
			throw runtime_error("no persons config file!");
		}

		World w(map, personsFileName, antennasConfigFileName);

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

//		cout << "... now we give mobile phones to persons... " << endl;
		// randomly select numMobilePhones people
//		unordered_set<int> indices;
//		while (indices.size() < numMobilePhones) {
//			int num = rand() % persons.size();
//			indices.insert(num);
//		}
//
//		vector<unsigned long> pindices;
//		std::move(indices.begin(), indices.end(), back_inserter(pindices));
//		unsigned long i = 0;
//		for (auto it = itr3.first; it != itr3.second; it++) {
//			MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
//			Person* p = persons[pindices[i++]];
//			m->setHolder(p);
//			p->addDevice(typeid(MobilePhone).name(), m);
//		}

//		cout << "Again the mobile phones but this time with assigned owners... " << endl;
//		utils::printPhoneHeader();
//		for (auto it = itr3.first; it != itr3.second; it++) {
//			MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
//			cout << m->toString() << endl;
//		}
//
//		cout << "Again the persons now with mobile phones... " << endl;
//		utils::printPersonHeader();
//		itr = c->getAgentListByType(typeid(Person).name());
//		for (auto it = itr.first; it != itr.second; it++) {
//			Person* p = dynamic_cast<Person*>(it->second);
//			cout << p->toString() << endl;
//		}

		string persFileName("persons.csv");
		string antennasFileName("antennas.csv");
		w.runSimulation(persFileName, antennasFileName);
	}
	catch (std::runtime_error& e) {
		cout << e.what() << "\n";
	}
	catch (const std::exception &e) {
		cout << e.what() << "\n";
	}
	return (0);
}
