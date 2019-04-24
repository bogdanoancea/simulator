/*
 * Data simulator for mobile phone network events
 *
 * EMField.h
 *
 *  Created on: Apr 23, 2019
 *      Author: Bogdan Oancea
 */

#ifndef EMFIELD_H_
#define EMFIELD_H_

#include <Antenna.h>
#include <Constants.h>
#include <utility>
#include <vector>

using namespace std;
/*
 * This singleton class will be used to compute the power of the electromagnetic field given by all antennas in a Point
 *
 */
class EMField {
	public:
		static EMField* instance() {
			if (m_instance == nullptr) {
				m_instance = new EMField();
			}
			return (m_instance);
		}
		/**
		 * Add an antenna that will contribute to the generation of the electromagnetic field
		 * @param a
		 */
		void addAntenna(Antenna* a);


		/**
		 * compute the power of the signal given by all antennas in location p
		 * @param p
		 * @return
		 */
		pair<Antenna*, double> computeMaxPower(Point* p);
		vector<pair<Antenna*, double>> getInRangeAntennas(Point* p, double powerThreshold = Constants::POWER_THRESHOLD);
		bool isAntennaInRange(Point* p, Antenna* a, double powerThreshold = Constants::POWER_THRESHOLD);

	private:
		EMField();

		EMField(const EMField&);
		EMField& operator=(const EMField&);

		static EMField* m_instance;
		vector<Antenna*> m_antennas;
};

#endif /* EMFIELD_H_ */
