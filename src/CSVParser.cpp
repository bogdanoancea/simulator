#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <CSVparser.hpp>

using namespace std;

Parser::Parser(const std::string &data, const DataType &type, char sep, bool hasHeader) :
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

Parser::~Parser(void) {
	vector<Row *>::iterator it;

	for (it = _content.begin(); it != _content.end(); it++)
		delete *it;
}

void Parser::parseHeader(void) {
	stringstream ss(_originalFile[0]);
	string item;

	while (getline(ss, item, _sep))
		_header.push_back(item);
}

void Parser::parseContent(void) {
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

Row &Parser::getRow(unsigned int rowPosition) const {
	if (rowPosition < _content.size())
		return *(_content[rowPosition]);
	throw runtime_error("can't return this row (doesn't exist)");
}

Row &Parser::operator[](unsigned int rowPosition) const {
	return Parser::getRow(rowPosition);
}

unsigned int Parser::rowCount(void) const {
	return _content.size();
}

unsigned int Parser::columnCount(void) const {
	return _header.size();
}

vector<string> Parser::getHeader(void) const {
	return _header;
}

const string Parser::getHeaderElement(unsigned int pos) const {
	if (pos >= _header.size())
		throw runtime_error("can't return this header (doesn't exist)");
	return _header[pos];
}

bool Parser::deleteRow(unsigned int pos) {
	if (pos < _content.size()) {
		delete *(_content.begin() + pos);
		_content.erase(_content.begin() + pos);
		return true;
	}
	return false;
}

bool Parser::addRow(unsigned int pos, const vector<string> &r) {
	Row *row = new Row(_header);

	for (auto it = r.begin(); it != r.end(); it++)
		row->push(*it);

	if (pos <= _content.size()) {
		_content.insert(_content.begin() + pos, row);
		return true;
	}
	return false;
}

void Parser::sync(void) const {
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

const string &Parser::getFileName(void) const {
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
