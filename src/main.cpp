#include <AgentsCollection.h>
#include <Antenna.h>
#include <geos/io/WKTWriter.h>
#include <InputParser.h>
#include <Map.h>
#include <MobilePhone.h>
#include <Person.h>
#include <Utils.h>
#include <World.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>
#include <Grid.h>
#include <CSVparser.hpp>
#include <AntennaInfo.h>
#include <iomanip>
#include <Constants.h>
#include<algorithm>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace utils;

int main(int argc, char** argv) {

	InputParser parser(argc, argv);
	if (argc == 2 && parser.cmdOptionExists("-h")) {
		cout
				<< "run this program like this: simulator -a <antennasConfigFile.xml> -m <mapFile.wkt> -p <personsConfigFile.xml> -s <simulationConfigFile> -v"
				<< endl;
		exit(0);
	}

	const string &antennasConfigFileName = parser.getCmdOption("-a");
	const string &mapFileName = parser.getCmdOption("-m");
	const string &personsConfigFileName = parser.getCmdOption("-p");
	const string &simulationConfigFileName = parser.getCmdOption("-s");
	bool verbose = parser.cmdOptionExists("-v");

	cout << "Hello from our mobile phone network simulator!" << endl;
	cout << "Now we are building the world!" << endl;

	try {
		Map* map;
		if (mapFileName.empty()) {
			throw runtime_error("no map file!");
		} else
			map = new Map(mapFileName);

		geos::io::WKTWriter writter;
		cout << "Our world has a map:" << endl
				<< writter.write(map->getBoundary()) << endl;

		if (antennasConfigFileName.empty()) {
			throw runtime_error("no antenna config file!");
		}
		if (personsConfigFileName.empty()) {
			throw runtime_error("no persons config file!");
		}

		if (simulationConfigFileName.empty()) {
			throw runtime_error("no simulation config file!");
		}

		World w(map, personsConfigFileName, antennasConfigFileName,
				simulationConfigFileName);

		AgentsCollection* c = w.getAgents();
		if (verbose) {
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
//		move(indices.begin(), indices.end(), back_inserter(pindices));
//		unsigned long i = 0;
//		for (auto it = itr3.first; it != itr3.second; it++) {
//			MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
//			Person* p = persons[pindices[i++]];
//			m->setHolder(p);
//			p->addDevice(typeid(MobilePhone).name(), m);
//		}

		w.runSimulation();

		time_t tt = w.getClock()->realTime();
		cout << "Computing probabilities started at " << ctime(&tt) << endl;
		//now we compute the probabilities for the positions of the phones

		// build the grid for the map
		Geometry* bbox = map->getBoundary()->getEnvelope();
		CoordinateSequence* seq = bbox->getCoordinates();
		double minX, minY, maxX, maxY;
		minX = minY = numeric_limits<double>::max();
		maxX = maxY = numeric_limits<double>::min();
		for (size_t i = 0; i < seq->size(); i++) {
			double x = seq->getX(i);
			double y = seq->getY(i);
			if (x > maxX)
				maxX = x;
			if (y > maxY)
				maxY = y;
			if (x < minX)
				minX = x;
			if (y < minY)
				minY = y;

		}
		double dimTileX = (maxX - minX) / 10;
		double dimTileY = (maxY - minY) / 10;
		Grid g(map, minX, minY, dimTileX, dimTileY, 10, 10);

		// read the event connection data
		vector<AntennaInfo> data;
		auto itra = c->getAgentListByType(typeid(Antenna).name());
		for (auto it = itra.first; it != itra.second; it++) {
			Antenna* a = dynamic_cast<Antenna*>(it->second);
			string fileName = a->getName() + to_string(a->getId()) + ".csv";
			Parser file = Parser(fileName, DataType::eFILE, ',', false);
			for (unsigned long i = 0; i < file.rowCount(); i++) {
				Row s = file[i];
				AntennaInfo a(stoul(s[0]), stoul(s[1]), stoul(s[2]),
						stoul(s[3]), stod(s[4]), stod(s[5]));
				data.push_back(a);
			}
		}
		sort(data.begin(), data.end(), [](const AntennaInfo& lhs, const AntennaInfo& rhs) {
		      return (lhs.getTime() < rhs.getTime() && lhs.getDeviceId() < rhs.getDeviceId());
		   });
//		sort(data.begin(), data.end());

		ofstream p_file, g_File;
		if (w.getProbFilename().empty()) {
			throw runtime_error("no output file!");
		}
		try {
			p_file.open(w.getProbFilename(), ios::out);
			g_File.open(w.getGridFilename(), ios::out);
		} catch (ofstream::failure& e) {
			cerr << "Error opening output file!" << endl;
		}
		g_File << g.toString();
		try {
			g_File.close();
		} catch (const ofstream::failure& e) {
			cerr << "Error closing grid file!" << endl;
		}

		w.getClock()->reset();
		auto itrm = c->getAgentListByType(typeid(MobilePhone).name());
		for (unsigned long t = w.getClock()->getInitialTime();
				t < w.getClock()->getFinalTime(); t = w.getClock()->tick()) {
			//iterate over all devices
			for (auto it = itrm.first; it != itrm.second; it++) {
				MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
				p_file << t << "," << m->getId() << ",";
				//ostringstream probs;
				for (unsigned long i = 0; i < g.getNoTiles(); i++) {
					p_file << fixed << setprecision(15) << g.computeProbability(t, i, m, data, itra) << ",";
				}
				//p_file << probs.str();
				p_file << endl;
			}
		}
		try {
			p_file.close();
		} catch (ofstream::failure& e) {
			cerr << "Error closing grid file!" << endl;
		}
		tt = w.getClock()->realTime();
		cout << "Computing probabilities ended at " << ctime(&tt) << endl;
	} catch (const runtime_error& e) {
		cout << e.what() << endl;
	} catch (const exception &e) {
		cout << e.what() << endl;
	}
	return (0);
}
