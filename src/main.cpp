#include <agent/AgentsCollection.h>
#include <geos/io/WKTWriter.h>
#include <InputParser.h>
#include <map/WKTMap.h>
#include <World.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <Utils.h>

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
		if (!generate_probs) {
			cout << "Location probabilities will be not computed!" << endl;
		} else {
			w.computeProbabilities();
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
