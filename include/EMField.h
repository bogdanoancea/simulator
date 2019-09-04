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
#include <map>

using namespace std;
/**
 * This utility singleton class is used to compute different measures of the electromagnetic field radiated by an antenna (power, signal strength etc)
 * and it also provides methods needed to decide to which antenna a mobile device connects.
 */
class EMField {
public:
	/**
	 * Returns an instance of this class. This class is a singleton.
	 * @return an instance of this class.
	 */
	static EMField* instance() {
		if (m_instance == nullptr) {
			m_instance = new EMField();
		}
		return (m_instance);
	}

	/**
	 * Default destructor.
	 */
	virtual ~EMField();

	/**
	 * Add a pointer to an Antenna object to an internal collection needed for computations. Although these pointers are kept in
	 * an AgentCollection object they are also added to a local vector in this class for performance reasons.
	 * @param a a pointer to the Antenna object
	 */
	void addAntenna(Antenna* a);

	/**
	 * Returns a pair made of a pointer to an Antenna object and its power with the property that in the location specified by @param p,
	 * the Antenna returned by this method provides the highest power (the power of the field is considered to decrease according a power-law.
	 * @param p the location where we want to find which Antenna provides the highest power the of electromagnetic field.
	 * @param mnoId the id of the MNO for which we compute the power. Only antennas belonging to this MNO will be considered during computations.
	 * @return a pair<Antenna*, double> containing a pointer to the Antenna object that provides the highest power of the field in @param p.
	 */
	pair<Antenna*, double> computeMaxPower(const Point* p, const unsigned long mnoId);

	/**
	 * Returns a pair made of a pointer to an Antenna object and its signal quality with the property that in the
	 * location specified by @param p, the Antenna returned by this method provides signal with the highest quality.
	 * The signal quality in this pair is the computed in location @param p.
	 * @param p the location where we want to find which Antenna provides the highest quality of the signal.
	 * @param mnoId the id of the MNO for which we compute the signal quality. Only antennas belonging to this MNO will be considered during computations.
	 * @return a pair<Antenna*, double> containing a pointer to the Antenna object that provides a signal with the
	 * highest quality in @param p.
	 */
	pair<Antenna*, double> computeMaxQuality(const Point* p, const unsigned long mnoId);

	/**
	 * Returns a vector of pairs made up of a pointer to an Antenna object and its power or signal quality.
	 * All the antennas in this vector provides a signal with a power or signal quality greater than
	 * the threshold provided as @param threshold, i.e. this vector contains all antennas that have in
	 * their coverage area the location given by point @param p.
	 * @param p the location where we want to have the list with the all antennas that covers it.
	 * @param threshold the lowest limit of the power or signal quality below which the signal is considered to be only
	 * noise, i.e. it defines the limit of the coverage area.
	 * @param power if true, the resulting vector of antennas is build according to the power of the field
	 * and the second element of the pair object represents the power of the antenna, otherwise it is
	 * build according to the signal quality and the second element of the pair is the signal quality of the antenna.
	 * @param mnoId the id of the MNO for which we build the resulting vector. Only antennas belonging to this MNO will be considered during computations.
	 * @return a vector of pairs made up of a pointer to an Antenna object and its power or signal quality.
	 * All the antennas in this vector provides a signal with a power or signal quality greater than the @param threshold.
	 */
	vector<pair<Antenna*, double>> getInRangeAntennas(const Point* p, const double threshold, const bool power, unsigned long mnoId);

	/**
	 * Checks if @param p is in the coverage area of Antenna pointed out by @param a. The covergare area is considered the area
	 * where the signal quality or the power of the field is greater than the value of @param threshold.
	 * @param p the location that we want to check the power or the quality of the signal
	 * @param a pointer to an Antenna object for which we want to check if it covers the point @param p.
	 * @param threshold the lower limit of the power or signal quality below which the signal is considered only noise.
	 * @param power if true the computations are done considering the power of the antenna, otherwise the signal quality.
	 * @return true is the Antenna object provide enough power or signal quality in the location given as @param p.
	 */
	bool isAntennaInRange(const Point* p, Antenna* a, const double threshold, const bool power);

	/**
	 * Computes the connection likelihood for Antenna indicated by @param a in a certain location given by @param p.
	 * The connection likelihood is computed dividing the signal quality provided by Antenna indicated through @param p by the sum
	 * of the signal quality provided by all antennas of an MNO.
	 * @param a a pointer to an Antenna object.
	 * @param p a location in space.
	 * @return the connection likelihood for Antenna @param a in @param p.
	 */
	double connectionLikelihood(Antenna* a, const Point * p);

	/**
	 * Computes the sum of the signal quality given by all antennas belonging to an MNO for all tiles in the reference grid.
	 * The signal quality is computed in the center of each tile.
	 * @param grid the grid of tiles where this method computes the sum of the signal quality. This grid is set at the beginning of
	 * the simulation and it overlaps the Map.
	 * @param mnoId the id of the MNO for which we want to compute this sum.
	 * @return a vector containing the sum of the signal quality given by all antennas of an MNO,
	 *  for all tiles in the reference grid. An element of the vector corresponds to a tile in the grid. The tiles
	 *  are linearized in a row-major order starting with the bottom-left corner.
	 */
	vector<double> sumSignalQuality(const Grid* grid, const unsigned long mnoID);

	/**
	 * Computes the connection likelihood for Antenna indicated by @param a in the center of the tile indicated by @param tileIndex
	 *
	 * @param a a pointer to an Antenna object.
	 * @param g a pointer to the reference Grid object
	 * @param tileIndex the index of the tile where we want to compute the connection likelihood.
	 * @return he connection likelihood for Antenna @param a in the center of the tile with the index @tileIndex.
	 */
	double connectionLikelihoodGrid(Antenna* a, const Grid* g, unsigned long tileIndex);

	const double* getAntennaMin3DbArray() const;

	double* getSd() const;

private:
	EMField();

	EMField(const EMField&);
	EMField& operator=(const EMField&);

	static EMField* m_instance;
	vector<Antenna*> m_antennas;
	map<const unsigned long, vector<double>> m_sumQuality;

	double* m_antennaMin3DbArray;
	double* m_sd;
};

#endif /* EMFIELD_H_ */
