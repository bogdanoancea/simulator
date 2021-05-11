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
 * Distribution.cpp
 *
 *  Created on: Apr 9, 2021
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <Constants.h>
#include <Distribution.h>
#include <string.h>
#include <stdexcept>
#include <Utils.h>

using namespace utils;



Distribution::Distribution(DistributionType type, std::vector<pair<const char*, double>> params) {
	m_type = type;
	m_params= params;

}

Distribution::Distribution(DistributionType type, XMLElement* element) {
	m_type = type;
	parseParams(type, element);
}


Distribution::~Distribution() {
	// TODO Auto-generated destructor stub
}

double Distribution::getParam(const char* name) {
	double result = 0;
	for(auto& x: m_params) {
		if(!strcmp(x.first, name))
			result = x.second;
	}
	return result;
}


bool Distribution::hasParam(const char* name) const {
	bool result = false;
	for(auto& x: m_params) {
		if(!strcmp(x.first, name))
			result = true;
	}
	return result;
}


string Distribution::paramsToString() {
	ostringstream result;
	for(auto& x: m_params) {
		result << x.first << ":" << x.second << endl;
	}
	return result.str();
}


DistributionType Distribution::getType() {
	return m_type;
}

void Distribution::setParams(vector<pair<const char*, double>> params) {
	m_params = params;
}

void Distribution::setParam(const char* name, double value) {
	if(!hasParam(name)) {
		std::pair<const char*, double> p1 = std::make_pair(name, value);
		m_params.push_back(p1);
	}
	else {
		for(auto& x: m_params) {
			if(!strcmp(x.first, name)) {
				x.second = value;
			}
		}
	}
}

void Distribution::parseParams(DistributionType type, XMLElement* element) {
	switch(type) {
	case DistributionType::NORMAL:
		parseNormalDistributionParams(element);
		break;
	case DistributionType::TRUNCATED_NORMAL:
		parseTruncatedNormalDistributionParams(element);
		break;
	case DistributionType::UNIFORM:
		parseUniformDistributionParams(element);
		break;
	case DistributionType::LAPLACE:
		parseLaplaceDistributionParams(element);
		break;
	case DistributionType::EXPONENTIAL:
		parseExponentialDistributionParams(element);
		break;
	case DistributionType::LEVY:
		parseLevyDistributionParams(element);
		break;
	default:
		throw std::runtime_error("Unknown distribution");
	}
}

void Distribution::parseNormalDistributionParams(XMLElement* el) {
	double mean = getValue(el, "mean", Constants::DEFAULT_MEAN_NORMAL);
	double sd = getValue(el, "sd", Constants::DEFAULT_SD_NORMAL);
	std::pair<const char*, double> p1 = std::make_pair("mean", mean);
	std::pair<const char*, double> p2 = std::make_pair("sd", sd);
	m_params.push_back(p1);
	m_params.push_back(p2);
}


void Distribution::parseTruncatedNormalDistributionParams(XMLElement* el) {
	double mean = getValue(el, "mean", Constants::DEFAULT_MEAN_NORMAL);
	double sd = getValue(el, "sd", Constants::DEFAULT_SD_NORMAL);
	double min = getValue(el, "min", Constants::DEFAULT_MIN_UNIFORM);
	double max = getValue(el, "max", Constants::DEFAULT_MAX_UNIFORM);
	std::pair<const char*, double> p1 = std::make_pair("mean", mean);
	std::pair<const char*, double> p2 = std::make_pair("sd", sd);
	std::pair<const char*, double> p3 = std::make_pair("min", min);
	std::pair<const char*, double> p4 = std::make_pair("max", max);
	m_params.push_back(p1);
	m_params.push_back(p2);
	m_params.push_back(p3);
	m_params.push_back(p4);
}

void Distribution::parseUniformDistributionParams(XMLElement* el) {
	double min = getValue(el, "min", Constants::DEFAULT_MIN_UNIFORM);
	double max = getValue(el, "max", Constants::DEFAULT_MAX_UNIFORM);
	std::pair<const char*, double> p1 = std::make_pair("min", min);
	std::pair<const char*, double> p2 = std::make_pair("max", max);
	m_params.push_back(p1);
	m_params.push_back(p2);
}


void Distribution::parseLevyDistributionParams(XMLElement* el) {
	double mean = Constants::DEFAULT_MEAN_LEVY;
	double c = getValue(el, "c", Constants::DEFAULT_C_LEVY);
	std::pair<const char*, double> p2 = std::make_pair("c", c);
	m_params.push_back(p2);
	try {
		mean = getValue(el, "mean", Constants::DEFAULT_MEAN_LEVY);
		std::pair<const char*, double> p1 = std::make_pair("mean", mean);
		m_params.push_back(p1);
	} catch(std::runtime_error &e) {
		// ignore, no mean, it will be set later
	}
}

void Distribution::parseLaplaceDistributionParams(XMLElement* el) {
	double scale = getValue(el, "scale", Constants::DEFAULT_SCALE_LAPLACE);
	const char* paramName = "scale";
	std::pair<const char*, double> p = std::make_pair(paramName, scale);
	m_params.push_back(p);
}

void Distribution::parseExponentialDistributionParams(XMLElement* el) {
	double param_value;
	const char* param_name;
	try {
		param_value = getValue(el, "scale");
		param_name = "scale";
	} catch (runtime_error& e) {
		param_value = getValue(el, "mean");
		param_name = "mean";
	}
	std::pair<const char*, double> p = std::make_pair(param_name, param_value);
	m_params.push_back(p);
}
vector<pair<const char*, double>>& Distribution::getParams() {
	return m_params;
}
