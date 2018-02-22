#include "nstdlib.h"
#include <locale.h>
#include <windows.h>
#include <queue>

nstd::TString::TString()
{
	_data = L'/0';
}

nstd::TString::TString(const string & data)
{
	_data = str2wstr(data);
}

nstd::TString::TString(const char * data)
{
	string str = data;
	_data = str2wstr(str);
}

nstd::TString::TString(const wchar_t * data)
{
	_data = data;
}

nstd::TString::TString(const wstring &data)
{
	_data = data;
}

nstd::TString nstd::TString::operator+(nstd::TString value)
{
	TString str;
	str._data = this->_data + value._data;
	return str;
}

nstd::TString nstd::TString::operator+(string value)
{
	TString str;
	str._data = this->_data + str2wstr(value);
	return str;
}

nstd::TString nstd::TString::operator+(wstring value)
{
	TString str;
	str._data = this->_data + value;
	return str;
}

nstd::TString nstd::TString::operator+(const char * value)
{
	TString str;
	str._data = this->_data + str2wstr(value);
	return str;
}

int nstd::TString::operator=(string value)
{
	this->_data = str2wstr(value);
	return 1;
}

int nstd::TString::operator=(wstring value)
{
	this->_data = value;
	return 1;
}

wchar_t  nstd::TString::operator[](int x)
{
	return _data.at(x);
}

nstd::TString & nstd::TString::operator+=(nstd::TString value)
{
	*this = *this + value;
	return *this;
}

nstd::TString & nstd::TString::operator+=(string value)
{
	*this = *this + value;
	return *this;
}

nstd::TString & nstd::TString::operator+=(wstring value)
{
	*this = *this + value;
	return *this;
}

nstd::TString & nstd::TString::operator+=(const char * value)
{
	*this = *this + value;
	return *this;
}

std::istream & nstd::TString::operator>>(std::istream & is)
{
	string str = wstr2str(_data);
	is >> str;
	*this = str;
	return is;
}

std::ostream & nstd::TString::operator<<(std::ostream & os)
{
	os << wstr2str(_data);
	return os;
}

std::ifstream & nstd::TString::operator>>(std::ifstream & is)
{
	// TODO: 於此處插入傳回陳述式
	return is;
}

std::ofstream & nstd::TString::operator<<(std::ofstream & os)
{
	// TODO: 於此處插入傳回陳述式
	return os;
}

string nstd::TString::wstr2str(std::wstring const & src)
{
	int len;
	int slength = (int)src.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, L'\0');
	WideCharToMultiByte(CP_ACP, 0, src.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

wstring nstd::TString::str2wstr(std::string const & src)
{
	int len;
	int slength = (int)src.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, src.c_str(), slength, &r[0], len);
	return r;
}

nstd::TString & nstd::TString::number(double num)
{
	std::stringstream ss;
	ss << num;
	_data = str2wstr(ss.str());
	return *this;
}



int nstd::TString::size()
{
	int n = 0;
	while (1) {
		if (_data[n] == '/0') {
			break;
		}
		else {
			++n;
		}
	}
	return n;
}

int nstd::TString::length()
{
	int n = 0;
	while (1) {
		if (_data[n] == '/0') {
			break;
		}
		else {
			++n;
		}
	}
	return n;
}

void nstd::TString::resize(int n)
{
	_data.resize(n);
}

void nstd::TString::clear()
{
	_data.clear();
}

bool nstd::TString::empty()
{
	return _data.empty();
}

wchar_t nstd::TString::at(int x)
{
	return _data.at(x);
}

wchar_t &nstd::TString::back()
{
	return _data.back();
}

wchar_t &nstd::TString::front()
{
	return _data.front();
}

void nstd::TString::append(TString value)
{
	_data.append(value._data);
}

void nstd::TString::append(string value)
{
	_data.append(str2wstr(value));
}

void nstd::TString::append(wstring value)
{
	_data.append(value);
}

wstring &nstd::TString::data()
{
	return _data;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
