#include <Map.h>
#include <World.h>
#include <iostream>
#include <iomanip>
#include <geos.h>

using namespace std;
using namespace geos;
using namespace geos::geom;

int main() {
	cout << "Hello from our mobile phone network simulator!" << endl;
	cout << "Now we are building the world!" << endl;
	Map* map = new Map(0, 0, 10, 10);
	geos::io::WKTWriter writter;
	cout << "Our world has a map:" << endl << writter.write(map->getBoundary()) << endl;

	int numPersons = 10;
	cout << "And it has " << numPersons << " persons!" << endl;
	World w(map, numPersons);

	cout << setw(10) << "Person ID" << setw(15) << " X " << setw(15) << " Y " << setw(15) << setw(15) << " Age " << endl;
	for (int i = 0; i < numPersons; i++) {
		AgentsCollection* c = w.getAgents();
		Person* p = dynamic_cast<Person*>(c->getAgent(i));
		cout << setw(10) << p->getId() << setw(15) << setw(15) << p->getLocation().getCoordinate()->x << setw(15)
				<< p->getLocation().getCoordinate()->y << setw(15) << setw(15) << p->getAge()
				<< endl;
	}
	w.runSimulation();

	return 0;
}
