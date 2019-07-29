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

/**
 * Utility singleton class to generate random numbers according to different distributions
 */
class RandomNumberGenerator {
public:
	/**
	 * Returns an instance of this class
	 * @return n instance of this class
	 */
	static RandomNumberGenerator* instance() {
		if (m_instance == nullptr) {
			m_instance = new RandomNumberGenerator();
		}
		return m_instance;
	}

	static RandomNumberGenerator* instance(unsigned seed) {
		if (m_instance == nullptr) {
			m_instance = new RandomNumberGenerator(seed);
		}
		return m_instance;
	}
//
//	uniform_int_distribution<int> getUnifIntDistribution() const {
//		return m_unif_int_distribution;
//	}
//
//	uniform_real_distribution<double> getUnifDoubleDistribution() const {
//		return m_unif_double_distribution;
//	}
//
//	normal_distribution<double> getNormalDoubleDistribution() const {
//		return m_normal_double_distribution;
//	}
//
//	std::mt19937& getGenerator() {
//		return m_generator;
//	}

	/**
	 * Generates n random number with a normal distribution. Half of them are N(m1,sd1), the other half N(m2,sd2)
	 * @param m1 the mean of the first normal distribution
	 * @param sd1 the standard deviation of the first normal distribution
	 * @param m2 the mean of the second normal distribution
	 * @param sd2 the standard deviation of the second normal distribution
	 * @param n the total number of values to be generated
	 * @return an array with random numbers according to two normal distributions
	 */
	double* generateNormal2Double(const double m1, const double sd1, const double m2, const double sd2, int n);

	/**
	 * Generates a random value, normally distributed with mean m and standard distribution sd
	 * @param m the mean of the normal distribution
	 * @param sd the standard deviation of the normal distribution
	 * @return a random value, normally distributed with mean m and standard distribution sd
	 */
	double generateNormalDouble(const double m, const double sd);

	/**
	 * Generates an array with n double values normally distributed with mean m and standard deviation sd
	 * @param m m the mean of the normal distribution
	 * @param sd the standard deviation of the normal distribution
	 * @param n the number of values to be generated
	 * @return an array with n double values normally distributed with mean m and standard deviation sd
	 */
	double* generateNormalDouble(const double m, const double sd, const int n);

	/**
	 * Generates n double values from a truncated normal distribution. All values will be in [a, b]
	 * @param a the inferior limit of the truncated normal distribution
	 * @param b the superior limit of the truncated normal distribution
	 * @param m the mean of the normal distribution
	 * @param sd the standard deviation of the normal distribution
	 * @param n the number of values to be generated
	 * @return an array with n double values from a truncated normal distribution
	 */
	double* generateTruncatedNormalDouble(const double a, const double b, const double m, const double sd, const unsigned long n);

	/**
	 * Generates a random double value from a uniform distribution which lies inside [min, max]
	 * @param min the lower limit of the value
	 * @param max the upper limit of the value
	 * @return a double value, uniformly distributed in [min, max]
	 */
	double generateUniformDouble(const double min, const double max);

	/**
	 * Generates n uniform distributed random values which lie inside [min, max]
	 * @param min the lower limit of the values
	 * @param max the upper limit of the values
	 * @param n the number of values to be generated
	 * @return n array with n double values from a uniform distribution
	 */
	double* generateUniformDouble(const double min, const double max, const int n);


	double generateExponentialDouble(const double lambda);

	/**
	 * Generates a random int value from a uniform distribution which lies inside [min, max]
	 * @param min the lower limit of the value
	 * @param max the upper limit of the value
	 * @return a int value, uniformly distributed in [min, max]
	 */
	int generateUniformInt(const int min, const int max);

	/**
	 * Generates n uniform distributed random values which lie inside [min, max]
	 * @param min the lower limit of the values
	 * @param max the upper limit of the values
	 * @param n the number of values to be generated
	 * @return n array with n int values from a uniform distribution
	 */
	int* generateUniformInt(const int min, const int max, const int n);

	/**
	 * Generates a int random value from a binomial distribution inside the interval [0, max]
	 * @param max the upper limit of the value
	 * @param p the parameter of the binomial distribution
	 * @return a int random value from a binomial distribution inside the interval [0, max]
	 */
	int generateBinomialInt(const int max, const double p);

	/**
	 * Generates a int random value from abernoulli distribution
	 * @param p the parameter of the bernoulli distribution
	 * @return a int random value from a bernoulii distribution
	 */
	int generateBernoulliInt(const double p);

	/**
	 * Generates n int random value from a binomial distribution inside the interval [0, max]
	 * @param max max the upper limit of the value
	 * @param p the parameter of the binomial distribution
	 * @param n the number of values to be generated
	 * @return array with n int values from a binomial distribution
	 */
	int* generateBinomialInt(const int max, const double p, const int n);

	/**
	 * Generates n int random values from a bernoulli distribution
	 * @param p the parameter of the bernoulli distribution
	 * @param n the number of values to be generated
	 * @return array with n int values from a bernoulli distribution
	 */
	int* generateBernoulliInt(const double p, const int n);

	//------------------------------------------------------------
	// Pseudorandom numbers from a truncated Gaussian distribution
	// The Gaussian has parameters mu (default 0) and sigma (default 1)
	// and is truncated on the interval [a,b].
	// Returns the random variable x and its probability p(x).
	//std::pair<double, double> generateTruncatedNormal(double a, double b, const double mu = 0, const double sigma = 1);
	//int N = 4001;

	//template<typename T> T normal_pdf(T x, T m, T s);
	double normal_pdf(double x, double m, double s) {
		const double inv_sqrt_2pi = 0.3989422804014327;
		double a = (x - m) / s;

		return inv_sqrt_2pi / s * exp(-(0.5) * a * a);
	}

	void setSeed(unsigned seed);

private:
	RandomNumberGenerator();
	RandomNumberGenerator(unsigned seed);
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
	exponential_distribution<double> m_exponential_double_distribution;
	binomial_distribution<int> m_binomial_distribution;
	bernoulli_distribution m_bernoulli_distribution;

	std::mt19937 m_generator;

};

#endif /* RANDOMNUMBERGENERATOR_H_ */
