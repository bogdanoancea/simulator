#include <agent/AgentsCollection.h>
#include <agent/Antenna.h>
#include <agent/HoldableAgent.h>
#include <agent/MobileOperator.h>
#include <agent/MobilePhone.h>
#include <AntennaInfo.h>
#include <Clock.h>
#include <Constants.h>
#include <CSVparser.hpp>
#include <EMField.h>
#include <geos/geom/Coordinate.h>
#include <geos/io/WKTWriter.h>
#include <InputParser.h>
#include <map/WKTMap.h>
#include <PriorType.h>
#include <Utils.h>
#include <World.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <new>
#include <sstream>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

//#if defined(__GNUC__) || defined(__GNUG__)
//#ifndef __clang__
//#include <omp.h>
//#include <parallel/algorithm>
//#include <parallel/settings.h>
//#endif
//#endif

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace utils;

int main(int argc, char** argv) {

//#if defined(__GNUC__) || defined(__GNUG__)
//#ifndef __clang__
//	const int threads_wanted = 8;
//	omp_set_dynamic(false);
//	omp_set_num_threads(threads_wanted);
//	__gnu_parallel ::_Settings s;
//	s.algorithm_strategy = __gnu_parallel::force_parallel;
//	__gnu_parallel::_Settings::set(s);
//#endif
//#endif

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
			map = new WKTMap(mapFileName);

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

		AgentsCollection* c = w.getAgents();
		if (verbose) {
			c->printAgents();
		}
		w.runSimulation();
		w.getMap()->dumpGrid(w.getGridFilename());
		std::map<unsigned long, vector<AntennaInfo>> data;
		auto itr_mno = c->getAgentListByType(typeid(MobileOperator).name());
		auto itra = c->getAgentListByType(typeid(Antenna).name());

		unsigned long noTiles = map->getNoTiles();
		Coordinate* tileCenters = w.getMap()->getTileCenters();
		for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
			MobileOperator* mo = static_cast<MobileOperator*>(itmno->second);
			vector<AntennaInfo> tmp;
			for (auto it = itra.first; it != itra.second; it++) {
				Antenna* a = static_cast<Antenna*>(it->second);
				if (a->getMNO()->getId() == mo->getId()) {
					ofstream& qualityFile = a->getMNO()->getSignalFile();
					qualityFile << a->getId() << sep;
					HoldableAgent::CONNECTION_TYPE handoverMechanism = w.getConnectionType();
					for (unsigned long i = 0; i < noTiles - 1; i++) {
						qualityFile << a->computeSignalMeasure(handoverMechanism, tileCenters[i]) << sep;
					}
					qualityFile << a->computeSignalMeasure(handoverMechanism, tileCenters[noTiles - 1]) << endl;

					string fileName = a->getAntennaOutputFileName();
					CSVParser file = CSVParser(fileName, DataType::eFILE, ',', true);
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
					antennaInfoFile << ai.toString() << sep << w.getMap()->getTileNo(ai.getX(), ai.getY()) << endl;
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
				cout << "Sum signal quality" << " MNO : " << itmno->second->getId() << endl;
				EMField::instance()->sumSignalQuality(map, itmno->second->getId());
			}

			ofstream p_file;
			unsigned long noTiles = w.getMap()->getNoTiles();
			for (auto itmno = itr_mno.first; itmno != itr_mno.second; itmno++) {
				Agent* mo = itmno->second;
				p_file.open(w.getProbFilenames()[mo->getId()], ios::out);
				p_file << "t" << sep << "Phone ID" << sep;
				for (unsigned long i = 0; i < noTiles - 1; i++) {
					p_file << "Tile" << i << sep;
				}
				p_file << "Tile" << noTiles - 1 << endl;
				w.getClock()->reset();
				for (unsigned long t = w.getClock()->getInitialTime(); t < w.getClock()->getFinalTime(); t = w.getClock()->tick()) {
					//iterate over all devices
					for (auto it = itrm.first; it != itrm.second; it++) {
						MobilePhone* m = dynamic_cast<MobilePhone*>(it->second);
						if (m->getMobileOperator()->getId() != mo->getId())
							continue;
						p_file << t << sep << m->getId() << sep;

						ostringstream probs;
						vector<double> p = utils::computeProbability(map, t, m, data[mo->getId()], itra, w.getPrior());
						for (unsigned long i = 0; i < map->getNoTiles() - 1; i++) {
							probs << fixed << setprecision(15) << p[i] << sep;
							//cout << p[i] << ",";
						}
						probs << fixed << setprecision(15) << p[map->getNoTiles() - 1];
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
