#ifndef     _CSVPARSER_HPP_
# define    _CSVPARSER_HPP_

# include <stdexcept>
# include <string>
# include <vector>
# include <list>
# include <sstream>

using namespace std;

class Error: public runtime_error {

public:
	Error(const string &msg) :
			runtime_error(string("CSVparser : ").append(msg)) {
	}
};

class Row {
public:
	Row(const vector<std::string> &);
	~Row(void);

public:
	unsigned int size(void) const;
	void push(const string &);
	bool set(const string &, const string &);

private:
	const vector<string> _header;
	vector<string> _values;

public:

	template<typename T>
	const T getValue(unsigned int pos) const {
		if (pos < _values.size()) {
			T res;
			stringstream ss;
			ss << _values[pos];
			ss >> res;
			return res;
		}
		throw Error("can't return this value (doesn't exist)");
	}
	const string operator[](unsigned int) const;
	const string operator[](const string &valueName) const;
	friend ostream& operator<<(ostream& os, const Row &row);
	friend ofstream& operator<<(ofstream& os, const Row &row);
};

enum DataType {
	eFILE = 0, ePURE = 1
};

class Parser {

public:
	Parser(const string &, const DataType &type = eFILE, char sep = ',', bool hasHeader = true);
	~Parser(void);

public:
	Row &getRow(unsigned int row) const;
	unsigned int rowCount(void) const;
	unsigned int columnCount(void) const;
	vector<string> getHeader(void) const;
	const string getHeaderElement(unsigned int pos) const;
	const string &getFileName(void) const;

public:
	bool deleteRow(unsigned int row);
	bool addRow(unsigned int pos, const vector<string> &);
	void sync(void) const;

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

public:
	Row &operator[](unsigned int row) const;
};

#endif /*!_CSVPARSER_HPP_*/
