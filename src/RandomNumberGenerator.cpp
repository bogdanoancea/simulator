/*
 * Data simulator for mobile phone network events
 *
 * RandomNumberGenerator.cpp
 *
 *  Created on: Apr 18, 2019
 *      Author: Bogdan Oancea
 */

#include <RandomNumberGenerator.h>

RandomNumberGenerator* RandomNumberGenerator::m_instance = nullptr;


RandomNumberGenerator::RandomNumberGenerator() {
	std::random_device device;
	m_generator.seed(device());
}

double* RandomNumberGenerator::generateNormal2Double(double m1, double sd1, double m2, double sd2, int n) {
	double* result = new double[n];
	//normal_distribution<double> d1 = m_normal_double_distribution;
	normal_distribution<double>::param_type p1(m1, sd1);
	normal_distribution<double>::param_type p2(m2, sd2);

	for (int i = 0; i < n; i++) {
		if (i % 2) {
			m_normal_double_distribution.param(p1);
			result[i] = m_normal_double_distribution(m_generator);
		}
		else {
			m_normal_double_distribution.param(p2);
			result[i] = m_normal_double_distribution(m_generator);
		}
	}
	return (result);
}

double RandomNumberGenerator::generateDouble(double min, double max) {
	double result = 0.0;
	uniform_real_distribution<double>::param_type p(min, max);
	m_unif_double_distribution.param(p);

	result = m_unif_double_distribution(m_generator);
	return (result);
}

double* RandomNumberGenerator::generateDouble(double min, double max, int n) {
	double* result = new double[n];
	uniform_real_distribution<double>::param_type p(min, max);
	m_unif_double_distribution.param(p);
	for (int i = 0; i < n; i++) {
		result[i] = m_unif_double_distribution(m_generator);
	}

	return result;
}
