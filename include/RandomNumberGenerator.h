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
		double generateNormalDouble(double m, double sd);
		double* generateNormalDouble(double m, double sd, int n);
		double* generateTruncatedNormalDouble(const double a, const double b, const double m, const double sd, const unsigned long n);
		double generateUniformDouble(double min, double max);
		double* generateUniformDouble(double min, double max, int n);
		int generateUniformInt(int min, int max);
		int* generateUniformInt(int min, int max, int n);
		int generateBinomialInt(int max, double p);
		int* generateBinomialInt(int max, double p, int n);

		//------------------------------------------------------------
		// Pseudorandom numbers from a truncated Gaussian distribution
		// The Gaussian has parameters mu (default 0) and sigma (default 1)
		// and is truncated on the interval [a,b].
		// Returns the random variable x and its probability p(x).
		//std::pair<double, double> generateTruncatedNormal(double a, double b, const double mu = 0, const double sigma = 1);
		//int N = 4001;

	private:
		RandomNumberGenerator();
		RandomNumberGenerator(const RandomNumberGenerator&);
		RandomNumberGenerator& operator=(const RandomNumberGenerator&);

		//------------------------------------------------------------
		// Compute y_l from y_k
//		double yl(int k);
//
//		//------------------------------------------------------------
//		// Rejection algorithm with a truncated exponential proposal
//		double rtexp(double a, double b);



		static RandomNumberGenerator* m_instance;
		uniform_int_distribution<int> m_unif_int_distribution;
		uniform_real_distribution<double> m_unif_double_distribution;
		normal_distribution<double> m_normal_double_distribution;
		binomial_distribution<int> m_binomial_distribution;

		std::mt19937 m_generator;

};

#endif /* RANDOMNUMBERGENERATOR_H_ */
