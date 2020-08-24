/*
 * Copyright (C) 2019  Bogdan Oancea

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version and under the EUPL free software license version 1.0 or later.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/> and
 * <https://ec.europa.eu/info/european-union-public-licence_en>
 *
 * A data simulator for mobile phone network events
 *
 * RandomNumberGenerator.cpp
 *
 *  Created on: Apr 18, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <RandomNumberGenerator.h>
#include <cmath>
#include <ctgmath>
#include <iostream>
#include <Utils.h>

using namespace std;
using namespace utils;

RandomNumberGenerator* RandomNumberGenerator::m_instance = nullptr;

RandomNumberGenerator::RandomNumberGenerator() {
	random_device device;
	m_generator.seed(device());
}

RandomNumberGenerator::RandomNumberGenerator(unsigned seed) {
	if(seed != 0)
		m_generator.seed(seed);
	else {
		random_device device;
		m_generator.seed(device());
	}
}

void RandomNumberGenerator::setSeed(unsigned seed) {
	m_instance->m_generator.seed(seed);
}


double* RandomNumberGenerator::generateNormal2Double(const double m1, const double sd1, const double m2, const double sd2, const int n) {
	double* result = new double[n];
	normal_distribution<double>::param_type p1(m1, sd1);
	normal_distribution<double>::param_type p2(m2, sd2);

	for (int i = 0; i < n; i++) {
		if (i % 2) {
			m_normal_double_distribution.param(p1);
			result[i] = m_normal_double_distribution(m_generator);
		} else {
			m_normal_double_distribution.param(p2);
			result[i] = m_normal_double_distribution(m_generator);
		}
	}
	return (result);
}

double RandomNumberGenerator::generateUniformDouble(const double min, const double max) {
	double result = 0.0;
	uniform_real_distribution<double>::param_type p(min, max);
	m_unif_double_distribution.param(p);

	result = m_unif_double_distribution(m_generator);
	return (result);
}

double* RandomNumberGenerator::generateUniformDouble(const double min, const double max, const int n) {
	double* result = new double[n];
	uniform_real_distribution<double>::param_type p(min, max);
	m_unif_double_distribution.param(p);
	for (int i = 0; i < n; i++) {
		result[i] = m_unif_double_distribution(m_generator);
	}
	return (result);
}

int RandomNumberGenerator::generateUniformInt(const int min, const int max) {
	int result = 0;
	uniform_int_distribution<int>::param_type p(min, max);
	m_unif_int_distribution.param(p);
	result = m_unif_int_distribution(m_generator);
	return (result);
}

int* RandomNumberGenerator::generateUniformInt(const int min, const int max, const int n) {
	int* result = new int[n];
	uniform_int_distribution<int>::param_type p(min, max);
	m_unif_int_distribution.param(p);
	for (int i = 0; i < n; i++) {
		result[i] = m_unif_int_distribution(m_generator);
	}
	return (result);
}

int RandomNumberGenerator::generateBinomialInt(const int max, const double p) {
	int result = 0;
	binomial_distribution<int>::param_type par(max, p);
	m_binomial_distribution.param(par);
	result = m_binomial_distribution(m_generator);
	return (result);
}

int* RandomNumberGenerator::generateBinomialInt(const int max, const double p, int n) {
	int* result = new int[n];
	binomial_distribution<int>::param_type par(max, p);
	m_binomial_distribution.param(par);
	for (int i = 0; i < n; i++) {
		result[i] = m_binomial_distribution(m_generator);
	}
	return (result);
}

double* RandomNumberGenerator::generateNormalDouble(const double m, const double sd, const int n) {
	double* result = new double[n];
	normal_distribution<double>::param_type p1(m, sd);
	m_normal_double_distribution.param(p1);
	for (int i = 0; i < n; i++) {
		result[i] = m_normal_double_distribution(m_generator);
	}
	return (result);
}

double* RandomNumberGenerator::generateTruncatedNormalDouble(const double a, const double b, const double m, const double sd,
		const unsigned long n) {
	double* result = new double[n];
	normal_distribution<double>::param_type p1(m, sd);
	m_normal_double_distribution.param(p1);
	unsigned long i = 0;
	while (i < n) {
		result[i] = m_normal_double_distribution(m_generator);
		if (result[i] >= a && result[i] <= b)
			i++;
	}
	return (result);
}

int* RandomNumberGenerator::generateTruncatedNormalInt(const int a, const int b, const double m, const double sd, const unsigned long n){
	int* result = new int[n];
	normal_distribution<double>::param_type p1(m, sd);
	m_normal_double_distribution.param(p1);
	unsigned long i = 0;
	while (i < n) {
		result[i] = (int)m_normal_double_distribution(m_generator);
		if (result[i] >= a && result[i] <= b)
			i++;
	}
	return (result);
}

double RandomNumberGenerator::generateNormalDouble(const double m, const double sd) {
	double result;
	normal_distribution<double>::param_type p1(m, sd);
	m_normal_double_distribution.param(p1);
	result = m_normal_double_distribution(m_generator);
	return (result);
}


double RandomNumberGenerator::generateExponentialDouble(const double lambda) {
	double result;
	exponential_distribution<double>::param_type p1(lambda);
	m_exponential_double_distribution.param(p1);
	result = m_exponential_double_distribution(m_generator);
	return (result);
}



int RandomNumberGenerator::generateBernoulliInt(const double p) {
	int result = 0;
	bernoulli_distribution::param_type par(p);
	m_bernoulli_distribution.param(par);
	result = m_bernoulli_distribution(m_generator);
	return (result);
}

int* RandomNumberGenerator::generateBernoulliInt(const double p, const int n) {
	int* result = new int[n];
	bernoulli_distribution::param_type par(p);
	m_bernoulli_distribution.param(par);
	for (int i = 0; i < n; i++) {
		result[i] = m_bernoulli_distribution(m_generator);
	}
	return (result);
}

double RandomNumberGenerator::normal_pdf(double x, double m, double s) {
	const double inv_sqrt_2pi = 0.3989422804014327;
	double a = (x - m) / s;

	return (inv_sqrt_2pi / s) * exp(-0.5 * a * a);
}

double* RandomNumberGenerator::generateLevy(const double mu, const double c, const int n) {
	double* result = new double[n];
	double* unif = generateUniformDouble(0, 1, n);
	for (int i = 0; i < n; i++) {
		double invNormCDF = utils::inverseNormalCDF(1-unif[i], 0, 1);
		result[i] = c / (2 * invNormCDF * invNormCDF) + mu;
	}
	return (result);
}

double RandomNumberGenerator::generateLevy(const double mu, const double c) {
	double result;
	double unif = generateUniformDouble(0, 1);
	double invNormCDF = utils::inverseNormalCDF(1-unif, 0, 1);
	result = c / (2 * invNormCDF * invNormCDF) + mu;
	return (result);
}
////------------------------------------------------------------
//// Compute y_l from y_k
//double RandomNumberGenerator::yl(int k) {
//	double yl0 = 0.053513975472;                  // y_l of the leftmost rectangle
//	double ylN = 0.000914116389555;               // y_l of the rightmost rectangle
//
//	if (k == 0)
//		return yl0;
//
//	else if (k == N - 1)
//		return ylN;
//
//	else if (k <= 1953)
//		return Rtnorm::yu[k - 1];
//
//	else
//		return Rtnorm::yu[k + 1];
//}
//
////------------------------------------------------------------
//// Rejection algorithm with a truncated exponential proposal
//double RandomNumberGenerator::rtexp(double a, double b) {
//	int stop = false;
//	double twoasq = 2 * pow(a, 2.0);
//	double expab = exp(-a * (b - a)) - 1;
//	double z, e;
//
//	while (!stop) {
//		z = log(1 + generateUniformDouble(0, 1) * expab);
//		e = -log(generateUniformDouble(0, 1));
//		stop = (twoasq * e > pow(z, 2.0));
//	}
//	return (a - z / a);
//}
//
////------------------------------------------------------------
//// Pseudorandom numbers from a truncated Gaussian distribution
//// The Gaussian has parameters mu (default 0) and sigma (default 1)
//// and is truncated on the interval [a,b].
//// Returns the random variable x and its probability p(x).
//std::pair<double, double> RandomNumberGenerator::generateTruncatedNormal(double a, double b, const double mu, const double sigma) {
//	double xmin = -2.00443204036;                 // Left bound
//	double xmax = 3.48672170399;                 // Right bound
//	int kmin = 5;                                 // if kb-ka < kmin then use a rejection algorithm
//	double INVH = 1631.73284006;                  // = 1/h, h being the minimal interval range
//	int I0 = 3271;                                // = - floor(x(0)/h)
//	double ALPHA = 1.837877066409345;             // = log(2*pi)
//	int xsize = sizeof(Rtnorm::x) / sizeof(double);   // Length of table x
//	int stop = false;
//	double sq2 = 7.071067811865475e-1;            // = 1/sqrt(2)
//	double sqpi = 1.772453850905516;              // = sqrt(pi)
//
//	double r, z, e, ylk, simy, lbound, u, d, sim, Z, p;
//	int i, ka, kb, k;
//
//	// Scaling
//	if (mu != 0 || sigma != 1) {
//		a = (a - mu) / sigma;
//		b = (b - mu) / sigma;
//	}
//
//	//-----------------------------
//
//	// Check if a < b
//	if (a >= b) {
//		cerr << "*** B must be greater than A ! ***" << endl;
//		exit(1);
//	}
//
//	// Check if |a| < |b|
//	else if (fabs(a) > fabs(b))
//		r = -generateTruncatedNormal(-b, -a).first;  // Pair (r,p)
//
//	// If a in the right tail (a > xmax), use rejection algorithm with a truncated exponential proposal
//	else if (a > xmax)
//		r = rtexp(a, b);
//
//	// If a in the left tail (a < xmin), use rejection algorithm with a Gaussian proposal
//	else if (a < xmin) {
//		while (!stop) {
//			r = generateNormalDouble(0, 1);
//			stop = (r >= a) && (r <= b);
//		}
//	}
//
//	// In other cases (xmin < a < xmax), use Chopin's algorithm
//	else {
//		// Compute ka
//		i = I0 + floor(a * INVH);
//		ka = Rtnorm::ncell[i];
//
//		// Compute kb
//		(b >= xmax) ? kb = N : (i = I0 + floor(b * INVH), kb = Rtnorm::ncell[i]);
//
//		// If |b-a| is small, use rejection algorithm with a truncated exponential proposal
//		if (abs(kb - ka) < kmin) {
//			r = rtexp(a, b);
//			stop = true;
//		}
//
//		while (!stop) {
//			// Sample integer between ka and kb
//			k = floor(generateUniformDouble(0, 1) * (kb - ka + 1)) + ka;
//
//			if (k == N) {
//				// Right tail
//				lbound = Rtnorm::x[xsize - 1];
//				z = -log(generateUniformDouble(0, 1));
//				e = -log(generateUniformDouble(0, 1));
//				z = z / lbound;
//
//				if ((pow(z, 2) <= 2 * e) && (z < b - lbound)) {
//					// Accept this proposition, otherwise reject
//					r = lbound + z;
//					stop = true;
//				}
//			}
//
//			else if ((k <= ka + 1) || (k >= kb - 1 && b < xmax)) {
//
//				// Two leftmost and rightmost regions
//				sim = Rtnorm::x[k] + (Rtnorm::x[k + 1] - Rtnorm::x[k]) * generateUniformDouble(0, 1);
//
//				if ((sim >= a) && (sim <= b)) {
//					// Accept this proposition, otherwise reject
//					simy = Rtnorm::yu[k] * generateUniformDouble(0, 1);
//					if ((simy < yl(k)) || (sim * sim + 2 * log(simy) + ALPHA) < 0) {
//						r = sim;
//						stop = true;
//					}
//				}
//			}
//
//			else // All the other boxes
//			{
//				u = generateUniformDouble(0, 1);
//				simy = Rtnorm::yu[k] * u;
//				d = Rtnorm::x[k + 1] - Rtnorm::x[k];
//				ylk = yl(k);
//				if (simy < ylk)  // That's what happens most of the time
//						{
//					r = Rtnorm::x[k] + u * d * Rtnorm::yu[k] / ylk;
//					stop = true;
//				}
//				else {
//					sim = Rtnorm::x[k] + d * generateUniformDouble(0, 1);
//
//					// Otherwise, check you're below the pdf curve
//					if ((sim * sim + 2 * log(simy) + ALPHA) < 0) {
//						r = sim;
//						stop = true;
//					}
//				}
//
//			}
//		}
//	}
//
//	//-----------------------------
//
//	// Scaling
//	if (mu != 0 || sigma != 1)
//		r = r * sigma + mu;
//
//	// Compute the probability
//	Z = sqpi * sq2 * sigma * (erf(b * sq2) - erf(a * sq2));
//	p = exp(-pow((r - mu) / sigma, 2) / 2) / Z;
//
//	return std::pair<double, double>(r, p);
//}
//
