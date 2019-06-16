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
#include <Grid.h>
#include <utility>
#include <vector>

using namespace std;
/**
 * This singleton class is used to compute the power of the electromagnetic field or the quality of the signal given by all antennas
 * used in a simulation in a given Point
 */
class EMField {
public:
	/**
	 * Returns an instance of this class
	 * @return an instance of this class
	 */
	static EMField* instance() {
		if (m_instance == nullptr) {
			m_instance = new EMField();
		}
		return (m_instance);
	}

	/**
	 * Add an antenna that will contribute to the generation of the electromagnetic field
	 * @param a a pointer to the Antenna object
	 */
	void addAntenna(Antenna* a);

	/**
	 * Returns a pair made of an Antenna object and its power with the property that in the location specified by Point p,
	 * among all Antennas that provide an electromagnetic field, the one given by the Antenna returned by this method has
	 * the highest power. The power in this pair is the power computed in location p.
	 * @param p the location where we want to find which is the Antenna with the highest power of electromagnetic field.
	 * @return a pair<Antenna*, double> containing a pointer to the Antenna object that provides an electromagnetic field with the
	 * highest power in p
	 */
	pair<Antenna*, double> computeMaxPower(const Point* p);

	/**
	 * Returns a pair made of an Antenna object and its signal quality with the property that in the location specified by Point p,
	 * among all Antennas that provide an electromagnetic field, the one given by the Antenna returned by this method has
	 * the highest quality of the signal. The signal quality in this pair is the computed in location p.
	 * @param p the location where we want to find which is the Antenna with the highest quality of the signal.
	 * @return a pair<Antenna*, double> containing a pointer to the Antenna object that provides a signal with the
	 * highest quality in p.
	 */
	pair<Antenna*, double> computeMaxQuality(const Point* p);

	/**
	 * Returns a vector of pairs made up of an Antenna object and its power or signal quality. All the antennas in this vector
	 * provides a signal with a power or signal quality greater than the threshold provided as a parameter
	 * @param p the location where we want to have the list with the antennas
	 * @param threshold the lowest limit of the power or signal quality below which the signal is considered to be only noise
	 * @param power if true, the vector of antennas is build according to the power of the electromagnetic field and the second element of the pair object
	 * represents the power of the antenna, otherwise it is
	 * build according to the signal quality and the second element of the pair is the signal quality of the antenna.
	 * @return a vector of pairs made up of an Antenna object and its power or signal quality. All the antennas in this vector
	 * provides a signal with a power or signal quality greater than the threshold provided as a parameter
	 */
	vector<pair<Antenna*, double>> getInRangeAntennas(const Point* p,
			const double threshold, const bool power);

	/**
	 * Checks is the power or the signal quality given by an antenna in a specific point in space is above the limit
	 * where it is considered only noise
	 * @param p the location in space where we want to check the power or the quality of the signal
	 * @param a a pointer to an Antenna object that we want to check if it provides enough power or signal quality in a point.
	 * @param threshold the lower limit of the power or signal quality below which the signal is considered only noise.
	 * @param power if true the computations are done considering the power of the antenna, otherwise the signal quality
	 * @return true is the provided Antenna object provide enough power or signal quality in the location given as a paramater.
	 */
	bool isAntennaInRange(const Point* p, Antenna* a, const double threshold, const bool power);

	/**
	 * Computes the connection likelihood. For a definition of this measure please consult the paper "Deriving geographic location of
	 * mobile devices from network data" by Martijn Tennekes, Yvonne A.P.M. Gootzen, Shan H. Shah.
	 * @param a a pointer to an Antenna object.
	 * @param p a location in space.
	 * @return the connection likelihood.
	 */
	double connectionLikelihood(Antenna* a, const Point * p);


	/**
	 * Computes the sum of the signal quality given by all antennas for all tiles in the reference grid
	 * @param grid the grid of tiles where this method coputes the sum of the signal quality
	 * @return a vector containing the sum ofthe signal quality given by all antennas,
	 *  for all tiles in the reference grid. An element of the vector corresponds to a tile in the grid. The tiles
	 *  are linearized in row-major order.
	 */
	vector<double>& sumSignalQuality(const Grid* grid);

	double connectionLikelihoodGrid(Antenna* a, const Grid* g, unsigned long tileIndex) const;

private:
	EMField();

	EMField(const EMField&);
	EMField& operator=(const EMField&);

	static EMField* m_instance;
	vector<Antenna*> m_antennas;
	vector<double> m_sumQuality;
};

#endif /* EMFIELD_H_ */
