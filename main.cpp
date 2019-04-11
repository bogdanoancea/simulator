#include <Map.h>
#include <World.h>
#include <iostream>
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

	cout << "Person ID" << '\t' << " X " << '\t' << " Y " << '\t' << '\t' << " Age " << endl;
	for (int i = 0; i < numPersons; i++) {
		AgentsCollection* c = w.getAgents();
		Person* p = dynamic_cast<Person*>(c->getAgent(i));
		cout << p->getId() << '\t' << '\t' << p->getLocation().getCoordinate()->x << '\t' << p->getLocation().getCoordinate()->y << '\t'
				<< '\t' << p->getAge()
				<< endl;
	}
	w.runSimulation();

	return 0;
}
