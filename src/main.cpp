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
#include <EMField.h>
#include <map>
#include <RandomNumberGenerator.h>

#if defined(__GNUC__) || defined(__GNUG__)
#ifndef __clang__
#include <omp.h>
#include <parallel/algorithm>
#include <parallel/settings.h>
#endif
#endif

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace utils;

int main(int argc, char** argv) {

#if defined(__GNUC__) || defined(__GNUG__)
#ifndef __clang__
	const int threads_wanted = 8;
	omp_set_dynamic(false);
	omp_set_num_threads(threads_wanted);
	__gnu_parallel ::_Settings s;
	s.algorithm_strategy = __gnu_parallel::force_parallel;
	__gnu_parallel::_Settings::set(s);
#endif
#endif

	InputParser parser(argc, argv);
	if (argc == 2 && parser.cmdOptionExists("-h")) {
		cout
				<< "run this program like this: simulator -a <antennasConfigFile.xml> -m <mapFile.wkt> -p <personsConfigFile.xml> -s <simulationConfigFile> -pb <probabilities.xml> -v -o"
				<< endl;
		exit(0);
	}
	char sep = Constants::sep;

	const string &antennasConfigFileName = parser.getCmdOption("-a");
	const string &mapFileName = parser.getCmdOption("-m");
	const string &personsConfigFileName = parser.getCmdOption("-p");
	const string &simulationConfigFileName = parser.getCmdOption("-s");
	const string & probabilitiesConfigFileName = parser.getCmdOption("-pb");
	bool verbose = parser.cmdOptionExists("-v");
	bool generate_probs = parser.cmdOptionExists("-o");
	cout << "Hello from our mobile phone network simulator!" << endl;
	cout << "Now we are building the world!" << endl;

	try {
		Map* map;
		if (mapFileName.empty())
			throw runtime_error("no map file!");
		else
			map = new Map(mapFileName);

		geos::io::WKTWriter writter;
		cout << "Our world has a map:" << endl << writter.write(map->getBoundary()) << endl;

		if (antennasConfigFileName.empty()) {
			throw runtime_error("no antenna config file!");
		}
		if (personsConfigFileName.empty()) {
			throw runtime_error("no persons config file!");
		}

		if (simulationConfigFileName.empty()) {
			throw runtime_error("no simulation config file!");
		}

		World w(map, personsConfigFileName, antennasConfigFileName, simulationConfigFileName, probabilitiesConfigFileName);
		map->addGrid(w.getGridDimTileX(), w.getGridDimTileY());

		AgentsCollection* c = w.getAgents();
		if (verbose) {
			utils::printMobileOperatorHeader();
			auto itr0 = c->getAgentListByType(typeid(MobileOperator).name());
			for (auto it = itr0.first; it != itr0.second; it++) {
				MobileOperator* mno = static_cast<MobileOperator*>(it->second);
				cout << mno->toString() << endl;
			}

			utils::printPersonHeader();
			auto itr = c->getAgentListByType(typeid(Person).name());
			vector<Person*> persons;
			for (auto it = itr.first; it != itr.second; it++) {
				Person* p = static_cast<Person*>(it->second);
				cout << p->toString() << endl;
			}

			utils::printAntennaHeader();
			auto itr2 = c->getAgentListByType(typeid(Antenna).name());
			for (auto it = itr2.first; it != itr2.second; it++) {
				Antenna* a = static_cast<Antenna*>(it->second);
				cout << a->toString() << endl;
			}

			utils::printPhoneHeader();
			auto itr3 = c->getAgentListByType(typeid(MobilePhone).name());
			for (auto it = itr3.first; it != itr3.second; it++) {
				MobilePhone* m = static_cast<MobilePhone*>(it->second);
				cout << m->toString() << endl;
			}
		}
		w.runSimulation();
		ofstream g_File;
		try {
			g_File.open(w.getGridFilename(), ios::out);
		} catch (ofstream::failure& e) {
			cerr << "Error opening grid output files!" << endl;
		}

		g_File << map->getGrid()->toString();
		try {
			g_File.close();
		} catch (const ofstream::failure& e) {
			cerr << "Error closing grid file!" << endl;
		}
		std::map<unsigned long, vector<AntennaInfo>> data;
		auto itr_mno = c->getAgentListByType(typeid(MobileOperator).name());
		auto itra = c->getAgentListByType(typeid(Antenna).name());

		vector<Coordinate> tileCenters;
		unsigned long noTiles = map->getGrid()->getNoTiles();
		for (unsigned long i = 0; i < noTiles - 1; i++) {
			Coordinate c = map->getGrid()->getTileCenter(i);
			c.z = 0;
			tileCenters.push_back(c);
		}
		for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
			MobileOperator* mo = static_cast<MobileOperator*>(itmno->second);
			vector<AntennaInfo> tmp;
			for (auto it = itra.first; it != itra.second; it++) {
				Antenna* a = static_cast<Antenna*>(it->second);
				if (a->getMNO()->getId() == mo->getId()) {
					ofstream& qualityFile = a->getMNO()->getSignalQualityFile();
					qualityFile << a->getId() << sep;
					for (unsigned long i = 0; i < noTiles - 1; i++) {
						qualityFile << a->computeSignalQuality(tileCenters[i]) << sep;
					}
					qualityFile << a->computeSignalQuality(tileCenters[noTiles - 1]) << endl;

					string fileName = a->getAntennaOutputFileName();
					Parser file = Parser(fileName, DataType::eFILE, ',', true);
					for (unsigned long i = 0; i < file.rowCount(); i++) {
						Row s = file[i];
						AntennaInfo a(stoul(s[0]), stoul(s[1]), stoul(s[2]), stoul(s[3]), stod(s[4]), stod(s[5]));
						tmp.push_back(a);
					}
				}
				sort(tmp.begin(), tmp.end());
				ofstream antennaInfoFile;
				string name = string("AntennaInfo_MNO_" + mo->getMNOName() + ".csv");
				antennaInfoFile.open(name, ios::out);
				antennaInfoFile << "t,Antenna ID,Event Code,Device ID,x,y, Tile ID" << endl;
				for (AntennaInfo& ai : tmp) {
					antennaInfoFile << ai.toString() << sep << w.getMap()->getGrid()->getTileNo(ai.getX(), ai.getY()) << endl;
				}
				antennaInfoFile.close();
			}
			data.insert(pair<unsigned long, vector<AntennaInfo>>(mo->getId(), tmp));
		}

		if (!generate_probs) {
			cout << "Location probabilities will be not computed!" << endl;
		} else {
			time_t tt = w.getClock()->realTime();
			cout << "Computing probabilities started at " << ctime(&tt) << endl;
			//now we compute the probabilities for the positions of the phones
			// read the event connection data

			w.getClock()->reset();
			auto itrm = c->getAgentListByType(typeid(MobilePhone).name());

			for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
				MobileOperator* mo = static_cast<MobileOperator*>(itmno->second);
				cout << "Sum signal quality" << " MNO : " << mo->getMNOName() << endl;
				EMField::instance()->sumSignalQuality(map->getGrid(), mo->getId());
			}

			ofstream p_file;
			const Grid* g = w.getMap()->getGrid();
			unsigned long noTiles = g->getNoTiles();
			for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
				MobileOperator* mo = dynamic_cast<MobileOperator*>(itmno->second);
				p_file.open(w.getProbFilenames()[mo->getId()], ios::out);
				p_file << "t" << sep << "Phone ID" << sep;
				for (unsigned long i = 0; i < noTiles - 1; i++) {
					p_file << "Tile" << i << sep;
				}
				p_file << "Tile" << noTiles - 1 << endl;

				for (unsigned long t = w.getClock()->getInitialTime(); t < w.getClock()->getFinalTime(); t = w.getClock()->tick()) {
					//iterate over all devices
					for (auto it = itrm.first; it != itrm.second; it++) {
						MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
						if (m->getMobileOperator()->getId() != mo->getId())
							continue;
						p_file << t << sep << m->getId() << sep;
						ostringstream probs;

						vector<double> p = map->getGrid()->computeProbability(t, m, data[mo->getId()], itra, w.getPrior());
						for (unsigned long i = 0; i < map->getGrid()->getNoTiles() - 1; i++) {
							probs << fixed << setprecision(15) << p[i] << sep;
							//cout << p[i] << ",";
						}
						probs << fixed << setprecision(15) << p[map->getGrid()->getNoTiles() - 1];
						//cout << p[map->getGrid()->getNoTiles() - 1] << endl;
						p_file << probs.str() << endl;
					}
				}
				try {
					p_file.close();
				} catch (ofstream::failure& e) {
					cerr << "Error closing probs file!" << endl;
				}
			}
			tt = w.getClock()->realTime();
			cout << "Computing probabilities ended at " << ctime(&tt) << endl;
		}
	} catch (const std::bad_alloc& e) {
		cout << e.what() << endl;
	} catch (const runtime_error& e) {
		cout << e.what() << endl;
	} catch (const exception &e) {
		cout << e.what() << endl;
	}
	return (0);
}
