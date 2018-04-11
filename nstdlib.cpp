#include "nstdlib.h"
#include <locale.h>
#include <windows.h>
#include <queue>

TString::TString()
{
	_data = L'/0';
}

TString::TString(const string & data)
{
	_data = str2wstr(data);
}

TString::TString(const char * data)
{
	string str = data;
	_data = str2wstr(str);
}

TString::TString(const wchar_t * data)
{
	_data = data;
}

TString::TString(const wstring &data)
{
	_data = data;
}

TString TString::operator+(TString value)
{
	TString str;
	str._data = this->_data + value._data;
	return str;
}

TString TString::operator+(string value)
{
	TString str;
	str._data = this->_data + str2wstr(value);
	return str;
}

TString TString::operator+(wstring value)
{
	TString str;
	str._data = this->_data + value;
	return str;
}

TString TString::operator+(const char * value)
{
	TString str;
	str._data = this->_data + str2wstr(value);
	return str;
}

int TString::operator=(string value)
{
	this->_data = str2wstr(value);
	return 1;
}

int TString::operator=(wstring value)
{
	this->_data = value;
	return 1;
}

wchar_t  TString::operator[](int x)
{
	return _data.at(x);
}

TString & TString::operator+=(TString value)
{
	*this = *this + value;
	return *this;
}

TString & TString::operator+=(string value)
{
	*this = *this + value;
	return *this;
}

TString & TString::operator+=(wstring value)
{
	*this = *this + value;
	return *this;
}

TString & TString::operator+=(const char * value)
{
	*this = *this + value;
	return *this;
}

bool TString::operator==(TString value)
{
	return (value._data == this->_data);
}

bool TString::operator==(string value)
{
	return (value == wstr2str(this->_data));
}

bool TString::operator==(wstring value)
{
	return (value == this->_data);
}

bool TString::operator==(const char * value)
{
	return (wstr2str(this->_data) == string(value));
}

std::istream & TString::operator>>(std::istream & is)
{
	string str = wstr2str(_data);
	is >> str;
	*this = str;
	return is;
}

std::ostream & TString::operator<<(std::ostream & os)
{
	os << wstr2str(_data);
	return os;
}

std::ifstream & TString::operator>>(std::ifstream & is)
{
	// TODO: 於此處插入傳回陳述式
	return is;
}

std::ofstream & TString::operator<<(std::ofstream & os)
{
	// TODO: 於此處插入傳回陳述式
	return os;
}

string TString::wstr2str(std::wstring const & src)
{
	int len;
	int slength = (int)src.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, L'\0');
	WideCharToMultiByte(CP_ACP, 0, src.c_str(), slength, &r[0], len, 0, 0);
	r.pop_back();
	return r;
}

wstring TString::str2wstr(std::string const & src)
{
	int len;
	int slength = (int)src.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, src.c_str(), slength, &r[0], len);
	r.pop_back();
	return r;
}

TString & TString::number(double num)
{
	std::stringstream ss;
	ss << num;
	_data = str2wstr(ss.str());
	return *this;
}

TString & TString::number(int num)
{
	std::stringstream ss;
	ss << num;
	_data = str2wstr(ss.str());
	return *this;
}



int TString::size()
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

int TString::length()
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

void TString::resize(int n)
{
	_data.resize(n);
}

void TString::clear()
{
	_data.clear();
}

bool TString::empty()
{
	return _data.empty();
}

wchar_t TString::at(int x)
{
	return _data.at(x);
}

wchar_t &TString::back()
{
	return _data.back();
}

wchar_t &TString::front()
{
	return _data.front();
}

void TString::append(TString value)
{
	_data.append(value._data);
}

void TString::append(string value)
{
	_data.append(str2wstr(value));
}

void TString::append(wstring value)
{
	_data.append(value);
}

wstring &TString::data()
{
	return _data;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

int bstrncmp(const char * s1, const char * s2, int n)
{
	int count = 0;
	if (*s1 == '\0' || *s2 == '0')
		return (*s1 - *s2);
	for (; *s1 != '\0'; s1++);
	for (; *s2 != '\0'; s2++);
	s1--;
	s2--;
	for (; *s1 != '\0'&&*s2 != '\0'&&count<n; s1--, s2--, count++) {
		if (*s1 - *s2 != 0)
			return (*s1 - *s2);
	}
	return 0;
}
