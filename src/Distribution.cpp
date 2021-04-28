/*
 * DistributionType.cpp
 *
 *  Created on: Apr 9, 2021
 *      Author: bogdan
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
	cout << "Truncated normal" << min << endl;
}

void Distribution::parseUniformDistributionParams(XMLElement* el) {
	double min = getValue(el, "min", Constants::DEFAULT_MIN_UNIFORM);
	double max = getValue(el, "max", Constants::DEFAULT_MAX_UNIFORM);
	std::pair<const char*, double> p1 = std::make_pair("min", min);
	std::pair<const char*, double> p2 = std::make_pair("max", max);
	m_params.push_back(p1);
	m_params.push_back(p2);
}

void Distribution::parseLaplaceDistributionParams(XMLElement* el) {
	double scale = getValue(el, "scale", Constants::DEFAULT_SCALE_LAPLACE);
	const char* paramName = "scale";
	std::pair<const char*, double> p = std::make_pair(paramName, scale);
	m_params.push_back(p);
}

vector<pair<const char*, double>>& Distribution::getParams() {
	return m_params;
}
