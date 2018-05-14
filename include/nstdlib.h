#ifndef NSTDLIB_H
#define NSTDLIB_H
#include <vector>
#include "Include.h"
#include <list>
#include <iostream>
#include <string>
#include <locale.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Entity.h"
using std::vector;
using std::list;
using std::string;
using std::wstring;

extern class Entity;


//namespace nstd {
	int bstrncmp(const char *s1, const char *s2, int n);

	//*****			template hash table			*****//
	template<typename T>
	class dic {
	public:
		string key;
		T value;
		dic(string Key, T Value) :key(Key), value(Value) {}
	};


	//		suitable size of table
	//		23 47 79 101 149 199 307 401 557 701 907
	template<typename T>
	class hash {
	public:
		hash(int m = 101) :size(m){
			table.resize(m);
		}

		void insert(dic<T> data) {
			int index = hashing(data.key);
			table[index].push_back(data);
		}

		void insert(string key, T value) {
			dic<T> data(key, value);
			int index = hashing(key);
			table[index].push_back(data);
		}

		T search(string key) {
			int index = hashing(key);
			for (list<dic<T>>::iterator itr = table[index].begin(); itr != table[index].end(); ++itr) {
				if ((*itr).key == key) {
					return ((*itr).value);
				}
			}
		}

		void deleteHash(string key) {
			int index = hashing(key);
			for (list<dic<T>>::iterator itr = table[index].begin(); itr != table[index].end(); ++itr) {
				if ((*itr).key == key) {
					table[index].erase(itr);
				}
			}
		}

		void resize(int size) {					//<----------------------------------untest
			vector<list<dic<T>>> newTable;
			newTable.resize(size);
			this->size = size;
			for (int i = 0; i < table.size(); ++i) {
				for (list<dic<T>>::iterator itr = table[i].begin(); itr != table[i].end(); ++itr) {
					dic<T> data(itr->key, itr->value);
					int index = hashing(itr->key);
					table[i].push_back(data);
				}
			}
			table = newTable;
		}
	private:
		vector<list<dic<T>>> table;
		int size;//size of table

		unsigned long long int preHashing(string key) {
			unsigned long long int exp = 8, key_value = 0, p = 1;
			for (int i = key.size(); i > 0; --i) {
				key_value += key[i] * p;
				p *= exp;
			}
			return key_value;
		}

		int hashing(string key) {
			return preHashing(key) % size;
		}
	};
	//*****			template hash table			*****//
	//*****			translation string			*****//
	class TString{
	public:
		TString();
		TString(const wstring &data);
		TString(const string &data);
		TString(const char *data);
		TString(const wchar_t *data);
		TString operator+(TString value);
		TString operator+(string value);
		TString operator+(wstring value);
		TString operator+(const char *value);
		int operator=(string value);
		int operator=(wstring value);
		wchar_t operator[](int x);
		TString &operator+=(TString value);
		TString &operator+=(string value);
		TString &operator+=(wstring value);
		TString &operator+=(const char *value);
		bool operator==(TString value);
		bool operator==(string value);
		bool operator==(wstring value);
		bool operator==(const char *value);
		std::istream &operator>>(std::istream &is);
		std::ostream &operator<<(std::ostream &os);
		std::ifstream &operator>>(std::ifstream &is);
		std::ofstream &operator<<(std::ofstream &os);
		string wstr2str(std::wstring const & src);
		wstring str2wstr(std::string const & src);
		TString &number(double num);		//this function will let the data be the number
		TString &number(int num);
		//size and length is same function with difference name
		int size();
		int length();
		void resize(int n);
		void clear();
		bool empty();
		wchar_t at(int x);
		wchar_t &back();
		wchar_t &front();
		void append(TString value);
		void append(string value);
		void append(wstring value);
		wstring &data();
		
	private:
		wstring _data;
	};
	//*****			translation string			*****//
	
//};

#endif