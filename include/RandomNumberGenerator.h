/*
 * Data simulator for mobile phone network events
 *
 * RandomNumberGenerator.h
 *
 *  Created on: Apr 18, 2019
 *      Author: Bogdan Oancea
 */

#ifndef RANDOMNUMBERGENERATOR_H_
#define RANDOMNUMBERGENERATOR_H_

#include <algorithm>
#include <random>

using namespace std;

class RandomNumberGenerator {
	public:
		static RandomNumberGenerator* instance() {
			if (m_instance == nullptr) {
				m_instance = new RandomNumberGenerator();
			}
			return m_instance;
		}

		uniform_int_distribution<int> getUnifIntDistribution() const {
			return m_unif_int_distribution;
		}

		uniform_real_distribution<double> getUnifDoubleDistribution() const {
			return m_unif_double_distribution;
		}

		normal_distribution<double> getNormalDoubleDistribution() const {
			return m_normal_double_distribution;
		}

		std::mt19937& getGenerator() {
			return m_generator;
		}

		double* generateNormal2Double(double m1, double sd1, double m2, double sd2, int n);
		double generateDouble(double min, double max);
		double* generateDouble(double min, double max, int n);
		int generateInt(int min, int max);
		int* generateInt(int min, int max, int n);


	private:
		RandomNumberGenerator();

		RandomNumberGenerator(const RandomNumberGenerator&);
		RandomNumberGenerator& operator=(const RandomNumberGenerator&);

		static RandomNumberGenerator* m_instance;
		uniform_int_distribution<int> m_unif_int_distribution;
		uniform_real_distribution<double> m_unif_double_distribution;
		normal_distribution<double> m_normal_double_distribution;
		std::mt19937 m_generator;

};

#endif /* RANDOMNUMBERGENERATOR_H_ */
