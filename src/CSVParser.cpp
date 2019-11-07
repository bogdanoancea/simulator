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
 * CSVParser.cpp
 *
 *  Created on: Apr 24, 2019
 *      Author: Bogdan Oancea
 *      Email : bogdan.oancea@gmail.com
 */

#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <CSVparser.hpp>

using namespace std;

CSVParser::CSVParser(const std::string &data, const DataType &type, char sep, bool hasHeader) :
		_type(type), _sep(sep), m_header { hasHeader } {
	string line;
	if (type == eFILE) {
		_file = data;
		fstream ifile(_file.c_str(), ios::in);
		if (ifile.is_open()) {
			while (ifile.good()) {
				getline(ifile, line);
				if (line != "")
					_originalFile.push_back(line);
			}
			ifile.close();

//			if (_originalFile.size() == 0)
//				throw runtime_error(string("No Data in ").append(_file));

			if (m_header)
				parseHeader();
			parseContent();
		} else
			throw runtime_error(string("Failed to open ").append(_file));
	} else {
		istringstream stream(data);
		while (getline(stream, line))
			if (line != "")
				_originalFile.push_back(line);
		if (_originalFile.size() == 0)
			throw runtime_error(string("No Data in pure content"));

		if (m_header)
			parseHeader();
		parseContent();
	}
}

CSVParser::~CSVParser(void) {
	vector<Row *>::iterator it;

	for (it = _content.begin(); it != _content.end(); it++)
		delete *it;
}

void CSVParser::parseHeader(void) {
	stringstream ss(_originalFile[0]);
	string item;

	while (getline(ss, item, _sep))
		_header.push_back(item);
}

void CSVParser::parseContent(void) {
	vector<string>::iterator it;

	it = _originalFile.begin();
	if (m_header)
		it++; // skip header

	for (; it != _originalFile.end(); it++) {
		bool quoted = false;
		int tokenStart = 0;
		unsigned int i = 0;

		Row *row = new Row(_header);

		for (; i != it->length(); i++) {
			if (it->at(i) == '"')
				quoted = ((quoted) ? (false) : (true));
			else if (it->at(i) == ',' && !quoted) {
				row->push(it->substr(tokenStart, i - tokenStart));
				tokenStart = i + 1;
			}
		}

		//end
		row->push(it->substr(tokenStart, it->length() - tokenStart));

		// if value(s) missing
		if (m_header && row->size() != _header.size())
			throw runtime_error("corrupted data !");
		_content.push_back(row);
	}
}

Row &CSVParser::getRow(unsigned int rowPosition) const {
	if (rowPosition < _content.size())
		return *(_content[rowPosition]);
	throw runtime_error("can't return this row (doesn't exist)");
}

Row &CSVParser::operator[](unsigned int rowPosition) const {
	return CSVParser::getRow(rowPosition);
}

unsigned int CSVParser::rowCount(void) const {
	return _content.size();
}

unsigned int CSVParser::columnCount(void) const {
	return _header.size();
}

vector<string> CSVParser::getHeader(void) const {
	return _header;
}

const string CSVParser::getHeaderElement(unsigned int pos) const {
	if (pos >= _header.size())
		throw runtime_error("can't return this header (doesn't exist)");
	return _header[pos];
}

bool CSVParser::deleteRow(unsigned int pos) {
	if (pos < _content.size()) {
		delete *(_content.begin() + pos);
		_content.erase(_content.begin() + pos);
		return true;
	}
	return false;
}

bool CSVParser::addRow(unsigned int pos, const vector<string> &r) {
	Row *row = new Row(_header);

	for (auto it = r.begin(); it != r.end(); it++)
		row->push(*it);

	if (pos <= _content.size()) {
		_content.insert(_content.begin() + pos, row);
		return true;
	}
	return false;
}

void CSVParser::sync(void) const {
	if (_type == DataType::eFILE) {
		ofstream f;
		f.open(_file, ios::out | ios::trunc);

		// header
		unsigned int i = 0;
		for (auto it = _header.begin(); it != _header.end(); it++) {
			f << *it;
			if (i < _header.size() - 1)
				f << ",";
			else
				f << endl;
			i++;
		}

		for (auto it = _content.begin(); it != _content.end(); it++)
			f << **it << endl;
		f.close();
	}
}

const string &CSVParser::getFileName(void) const {
	return _file;
}

/*
 ** ROW
 */

Row::Row(const vector<string> &header) :
		_header(header) {
}

Row::~Row(void) {
}

unsigned int Row::size(void) const {
	return _values.size();
}

void Row::push(const std::string &value) {
	_values.push_back(value);
}

bool Row::set(const string &key, const string &value) {
	vector<string>::const_iterator it;
	int pos = 0;

	for (it = _header.begin(); it != _header.end(); it++) {
		if (key == *it) {
			_values[pos] = value;
			return true;
		}
		pos++;
	}
	return false;
}

const string Row::operator[](unsigned int valuePosition) const {
	if (valuePosition < _values.size())
		return _values[valuePosition];
	throw runtime_error("can't return this value (doesn't exist)");
}

const string Row::operator[](const string &key) const {
	vector<std::string>::const_iterator it;
	int pos = 0;

	for (it = _header.begin(); it != _header.end(); it++) {
		if (key == *it)
			return _values[pos];
		pos++;
	}

	throw runtime_error("can't return this value (doesn't exist)");
}

ostream &operator<<(ostream &os, const Row &row) {
	for (unsigned int i = 0; i != row._values.size(); i++)
		os << row._values[i] << " | ";

	return os;
}

ofstream &operator<<(ofstream &os, const Row &row) {
	for (unsigned int i = 0; i != row._values.size(); i++) {
		os << row._values[i];
		if (i < row._values.size() - 1)
			os << ",";
	}
	return os;
}
