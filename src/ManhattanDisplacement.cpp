/*
 * ManhattanDisplacement.cpp
 *
 *  Created on: May 27, 2021
 *      Author: bogdan
 */

#include <ManhattanDisplacement.h>
#include <RandomNumberGenerator.h>
#include <Utils.h>

ManhattanDisplacement::ManhattanDisplacement(SimulationConfiguration *simConfig, double speed) :
		Displace(simConfig, speed) {
	m_manhattanScenario = simConfig->getManhattanScenario();
	m_status = STATE::OUTSIDE;
	m_theta = Directions::EAST;
	m_distance = m_speed * m_simConfig->getClock()->getIncrement();
	m_direction = -1;
}

ManhattanDisplacement::~ManhattanDisplacement() {
	// TODO Auto-generated destructor stub
}

Point* ManhattanDisplacement::generateNewLocation(Point * initLocation) {
	Point* result;
	const Coordinate* initC = initLocation->getCoordinate();
	double x = initC->x;
	double y = initC->y;
	Coordinate current = Coordinate(x,y,0.0);
	STATE savedStatus = m_status;
	Directions savedTheta  = m_theta;
//cout << "generez 1" << endl;
	if(m_simConfig->getClock()->getCurrentTime() == m_simConfig->getClock()->getInitialTime() +  m_simConfig->getClock()->getIncrement()) {
		current = closestCorner(*initC);
		result = m_simConfig->getMap()->getGlobalFactory()->createPoint(current);
		m_status = STATE::ONCORNER;
	} else {
		//cout << "generez 2" << endl;
		double distanceToGo = m_distance;
		if(m_status == STATE::ONCORNER) {
			startFromCorner(current, distanceToGo, m_theta, m_status);
		}
		else if(m_status == STATE::ONEDGE) {
			startFromEdge(current, distanceToGo, m_theta, m_status);
		}
//		cout << "generez 3" << endl;
		result = m_simConfig->getMap()->getGlobalFactory()->createPoint(current);
		Geometry* g = m_simConfig->getMap()->getBoundary();
		if (!result->within(g)) {
			m_simConfig->getMap()->getGlobalFactory()->destroyGeometry(result);
			result = initLocation;
			m_status = savedStatus;
			if(m_status == STATE::ONCORNER)
				m_theta = savedTheta;
			else {
				m_theta = reverseDirection(savedTheta);
			}
		}
	}
	return result;

}

void ManhattanDisplacement::startFromCorner(Coordinate &current, double distanceToGo, Directions &angle, ManhattanDisplacement::STATE &status) {
	while (distanceToGo > 0) {
		if (status == STATE::ONCORNER)
			angle = selectDirection();
		double stepLDir = blockLength(angle);
		if (stepLDir < distanceToGo) {
			current = makeBlockStep(current, angle);
			distanceToGo -= stepLDir;
			status = STATE::ONCORNER;
		} else {
			distanceToGo = makeMultipleBlocksStep(current, distanceToGo, angle, status);
		}
	}
}


void ManhattanDisplacement::startFromEdge(Coordinate &current, double distanceToGo, Directions &angle, ManhattanDisplacement::STATE &status) {
	Coordinate previous;
	while (distanceToGo > 0) {
		if (m_status == STATE::ONCORNER)
			m_theta = selectDirection();
		double stepLDir = blockLength(m_theta);
		if (stepLDir < distanceToGo) {
			previous = current;
			current = makeBlockStep(current, m_theta);
			distanceToGo -= stepLDir;
			Coordinate corner = closestCorner(current);
			if (!(doubleCompare(corner.x, previous.x)
					&& doubleCompare(corner.y, previous.y))) {
				if (atCorner(current, corner)) {
					current = corner;
					m_status = STATE::ONCORNER;
				} else {
					m_status = STATE::ONEDGE;
				}
			} else {
				m_status = STATE::ONEDGE;
			}
		} else {
			distanceToGo = makeMultipleBlocksStep(current, distanceToGo, m_theta, m_status);
		}
	}
}



double ManhattanDisplacement::makeMultipleBlocksStep(Coordinate& current, double distanceToGo, Directions& angle, ManhattanDisplacement::STATE& status)  {
	Coordinate previous = current;
	current = makeIncompleteStep(current, angle, distanceToGo);
	Coordinate corner = closestCorner(current);
	if( !(doubleCompare(corner.x, previous.x) &&  doubleCompare(corner.y, previous.y)) ) {
		if(atCorner(current, corner)) {
			current = corner;
			status = STATE::ONCORNER;

		} else {
			status = STATE::ONEDGE;
		}
	}
	else {
		status = STATE::ONEDGE;
	}
	return -1;
}


Coordinate ManhattanDisplacement::closestCorner(Coordinate location) const {
	double xcorner, ycorner;
	double x = location.x;
	double y = location.y;
	//cout << "generez 2.1" << endl;
	double x1 = floor((x - m_manhattanScenario->getXOrigin()) / m_manhattanScenario->getXStep()) * m_manhattanScenario->getXStep();
	double x2 = ceil((x - m_manhattanScenario->getXOrigin()) / m_manhattanScenario->getXStep()) * m_manhattanScenario->getXStep();
	double y1 = floor((y - m_manhattanScenario->getYOrigin()) / m_manhattanScenario->getYStep()) * m_manhattanScenario->getYStep();
	double y2 = ceil((y - m_manhattanScenario->getYOrigin()) / m_manhattanScenario->getYStep()) * m_manhattanScenario->getYStep();
	//cout << "generez 2.2" << endl;
	if(fabs(x-x1) < fabs(x-x2))
		xcorner = x1;
	else
		xcorner = x2;

	if(fabs(y-y1) < fabs(y-y2))
		ycorner = y1;
	else
		ycorner = y2;

	Coordinate c1 = Coordinate(xcorner, ycorner, 0.0);
	return c1;
}

Directions ManhattanDisplacement::selectDirection() const{
	Directions result;
	RandomNumberGenerator* rng = RandomNumberGenerator::instance();
	if(m_direction  >= 0 ) {
		if(m_direction == 0.0)
			result = Directions::EAST;
		else if(m_direction > 0.0 && m_direction < utils::PI / 2) {
			unsigned d = rng->generateUniformInt(0, 1);
			if(d == 0)
				result = Directions::EAST;
			else if(d == 1)
				result = Directions::NORTH;
		}
		else if(m_direction == utils::PI)
			result = Directions::NORTH;
		else if(m_direction > utils::PI / 2 && m_direction < utils::PI) {
			unsigned d = rng->generateUniformInt(0, 1);
			if(d == 0)
				result = Directions::NORTH;
			else if(d == 1)
				result = Directions::WEST;
		} else if( m_direction == utils::PI)
			result = Directions::WEST;
		else if(m_direction > utils::PI && m_direction < 3.0 * utils::PI / 2.0) {
			unsigned d = rng->generateUniformInt(0, 1);
			if(d == 0)
				result = Directions::WEST;
			else if(d == 1)
				result = Directions::SOUTH;
		}
		else if(m_direction == 3.0 * utils::PI / 2.0)
			result = Directions::SOUTH;
		else if(m_direction > 3.0 * utils::PI / 2.0 && m_direction < 2 * utils::PI) {
			unsigned d = rng->generateUniformInt(0, 1);
			if(d == 0)
				result = Directions::SOUTH;
			else if(d == 1)
				result = Directions::EAST;
		}
	}
	else {
		unsigned theta = rng->generateUniformInt(0, 3);
		if(theta == 0)
			result = Directions::EAST;
		else if(theta == 1)
			result = Directions::NORTH;
		else if(theta == 2)
			result = Directions::WEST;
		else
			result = Directions::SOUTH;
		}
	return result;
}

bool ManhattanDisplacement::atCorner(Coordinate pos, Coordinate corner) const {

	double distToCorner = 0.0;
	if(doubleCompare(pos.x, corner.x))
		distToCorner = fabs(pos.y - corner.y);
	else if(doubleCompare(pos.y, corner.y))
		distToCorner = fabs(pos.x - corner.x);
	else if (doubleCompare(pos.x, corner.x) && doubleCompare(pos.y, corner.y))
		distToCorner = 0.0;
	else
		distToCorner = sqrt(pow(pos.x - corner.x, 2) + pow(pos.y - corner.y, 2));

	if(distToCorner < m_distance)
		return true;
	else
		return false;
}

bool ManhattanDisplacement::doubleCompare(double x, double y) const {
    double maxXYOne = std::max( { 1.0, std::fabs(x) , std::fabs(y) } ) ;
    return std::fabs(x - y) <= std::numeric_limits<double>::epsilon()*maxXYOne ;
}

Coordinate ManhattanDisplacement::makeBlockStep(Coordinate location, Directions theta) const{
	double x = location.x;
	double y = location.y;
	double xstep  = m_manhattanScenario->getXStep();
	double ystep  = m_manhattanScenario->getYStep();
	if(theta == Directions::EAST) {
		x += xstep;
	} else if(theta == Directions::NORTH) {
		y += ystep;
	}
	else if(theta == Directions::WEST) {
		x -= xstep;
	}
	else if(theta == Directions::SOUTH ){
		y -= ystep;
	}
	return Coordinate(x, y, 0.0);
}


Coordinate ManhattanDisplacement::makeIncompleteStep(Coordinate location, Directions theta, double length) const {
	double x = location.x;
	double y = location.y;
	if(theta == Directions::EAST) {
		x += length;
	} else if(theta == Directions::NORTH) {
		y += length;
	}
	else if(theta == Directions::WEST) {
		x -= length;
	}
	else if(theta == Directions::SOUTH) {
		y -= length;
	}
	Coordinate c = Coordinate(x, y, 0.0);
	return c;
}

double ManhattanDisplacement::blockLength(Directions theta) const {
	double result = 0;
	if( theta == Directions::EAST || theta == Directions::WEST)
		result = m_manhattanScenario->getXStep();
	else if(theta == Directions::NORTH || theta == Directions::SOUTH) {
		result = m_manhattanScenario->getYStep();
	}
	return result;
}

Directions ManhattanDisplacement::reverseDirection(Directions dir) const {
	Directions result;
	if(dir == Directions::SOUTH)
		result = Directions::NORTH;
	else if(dir == Directions::EAST)
		result = Directions::WEST;
	else if(dir == Directions::NORTH)
		result = Directions::SOUTH;
	else
		result = Directions::EAST;
	return result;
}

//STATE ManhattanDisplacement::checkStatus(Point* location) const {
//	STATE result;
//	double x = location->getX();
//	double y = location->getY();
//	double xManhattanStep =  m_manhattanScenario->getXStep();
//	double yManhattanStep =  m_manhattanScenario->getYStep();
//	if(compareDouble(fmod(x, xManhattanStep), 0.0) && compareDouble(fmod(y, yManhattanStep), 0.0)){
//		result = STATE::ONCORNER;
//	}
//	else if(compareDouble(fmod(x, xManhattanStep), 0.0) ||compareDouble(fmod(y, yManhattanStep), 0.0)) {
//		result = STATE::ONEDGE
//	}
//	else {
//		result = STATE::OUTSIDE;
//	}
//	return result;
//}

ManhattanDisplacement::STATE ManhattanDisplacement::getStatus() const {
	return m_status;
}

void ManhattanDisplacement::setStatus(STATE status) {
	m_status = status;
}

void ManhattanDisplacement::setDirection(double direction) {
	m_direction = direction;
}

double ManhattanDisplacement::getDirection() const {
	return m_direction;
}

void ManhattanDisplacement::setSpeed(double speed) {
	m_speed = speed;
	m_distance = m_speed * m_simConfig->getClock()->getIncrement();
}
