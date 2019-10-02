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
 *      Email : bogdan.oancea@gmail.com
 */

#ifndef CSVPARSER_HPP
#define CSVPARSER_HPP

# include <stdexcept>
# include <string>
# include <vector>
# include <list>
# include <sstream>

using namespace std;

/**
 * This class is used to represent a line from a .csv file. It is a container of the values from a line of text.
 */
class Row {
public:
	/**
	 * Constructor. It takes a a line of text representing the header of the csv file.
	 * @param header the header line
	 */
	Row(const vector<string> &header);

	/**
	 * Destructor.
	 */
	~Row(void);

public:
	/**
	 * Returns the number of values on a row.
	 * @return the number of values on a row.
	 */
	unsigned int size(void) const;

	/**
	 * Add a new value on a row
	 * @param value to be added
	 */
	void push(const string &value);

	/**
	 * Sets the value of a specific element in a row. The element is specified by the column name.
	 * @param key the name of the column (given in the header of the file)
	 * @param value the value to be set
	 * @return
	 */
	bool set(const string& key, const string& value);

private:
	const vector<string> _header;
	vector<string> _values;

public:

	/**
	 * Returns the value of an element from a row
	 * @param pos the number of the element in row
	 * @return the value
	 */
	template<typename T>
	const T getValue(unsigned int pos) const {
		if (pos < _values.size()) {
			T res;
			stringstream ss;
			ss << _values[pos];
			ss >> res;
			return res;
		}
		throw runtime_error("can't return this value (doesn't exist)");
	}

	/**
	 * Overloaded operator
	 * @param i the position of the value that we want to extract
	 * @return the value from position i
	 */
	const string operator[](unsigned int i) const;

	/**
	 * Overloaded operator
	 * @param valueName the name of the column from which we want to extract a the value
	 * @return the value from the column specified by its name
	 */
	const string operator[](const string &valueName) const;

	/**
	 * Overloaded operator outputs the content of a row in a stream
	 * @param os the stream where we want to send the content of the row.
	 * @param row the row that we want to output
	 * @return the same output stream
	 */
	friend ostream& operator<<(ostream& os, const Row &row);

	/**
	 * Overloaded operator outputs the content of a row in a file stream
	 * @param os the file stream where we want to send the content of the row.
	 * @param row the row that we want to output
	 * @return the same output file stream
	 */
	friend ofstream& operator<<(ofstream& os, const Row &row);
};

enum DataType {
	eFILE = 0, ePURE = 1
};

/**
 * This class is used to read and parse a csv file or to write some values as a csv file.
 */
class Parser {

public:
	/**
	 * Constructor of the class. It need the name of the csv file, the file type,
	 * the separator and a boolean that indicates if the file has header or not.
	 * @param data the name of the file
	 * @param type the file type: could be eFILE for normal text files or ePURE if the input is a string
	 * @param sep the separator of the individula values in a line of the csv file
	 * @param hasHeader true means that the csv file has a header line, false that it doesn't have a header
	 */
	Parser(const string &data, const DataType &type = eFILE, char sep = ',',
			bool hasHeader = true);

	/**
	 * Destructor
	 */
	~Parser(void);

	/**
	 * Returns a Row object specified by its number in the file
	 * @param row the number of the line that was used to build the Row object
	 * @return a Row object specified by its number in the file
	 */
	Row &getRow(unsigned int row) const;

	/**
	 * Returns the number of lines in the csv file without counting the header line, if it exists
	 * @return the number of lines in the csv file without counting the header line, if it exists
	 */
	unsigned int rowCount(void) const;

	/**
	 * Returns the number of the columns of the csv file.
	 * @return the number of the columns of the csv file.
	 */
	unsigned int columnCount(void) const;

	/**
	 * Returns a vector containing the names of the columns as they are specified in the header line of the csv file.
	 * @return a vector containing the names of the columns as they are specified in the header line of the csv file.
	 */
	vector<string> getHeader(void) const;

	/**
	 * Returns the name of a specific column given by its position in the header line
	 * @param pos the number of the column
	 * @return the name of a specific column given by its position in the header line
	 */
	const string getHeaderElement(unsigned int pos) const;

	/**
	 * Returns the name of the csv file
	 * @return the name of the csv file
	 */
	const string &getFileName(void) const;

	/**
	 * Removes a row specified by its number
	 * @param row the number of the row to be deleted
	 * @return true if the removal succeeded, false otherwise
	 */
	bool deleteRow(unsigned int row);

	/**
	 * Inserts a Row object at a given position
	 * @param pos the position where we want to insert the Row object
	 * @param r a vector containing the values in the Row.
	 * @return true if the insertion is successful, false otherwise (i.e. the pos parameter is outside the limits of the container that stores the Rows of the csv file.
	 */
	bool addRow(unsigned int pos, const vector<string> &r);

	/**
	 *Flushes the content to a file on disk and then closes the file.
	 */
	void sync(void) const;

	/**
	 * Overloaded operator
	 * @param row the number of the row to be retrieved
	 * @return the Row object at the position specified by row
	 */
	Row &operator[](unsigned int row) const;

protected:
	void parseHeader(void);
	void parseContent(void);

private:
	string _file;
	const DataType _type;
	const char _sep;
	vector<string> _originalFile;
	vector<string> _header;
	vector<Row *> _content;
	bool m_header;
};

#endif /*CSVPARSER_HPP*/
