#ifndef NSTDLIB_H
#define NSTDLIB_H
#include <vector>
#include <list>
#include <string>
#include "Shader.h"
using std::vector;
using std::list;
using std::string;

namespace nstd {
	//*****			template hash table			*****//
	template<typename T>
	class dic {
	public:
		string key;
		T value;
		dic(string Key, T Value);
	};
	template<typename T>
	class hash {
	public:
		hash(int m = 100);
		void insert(dic<T> data);
		T search(string key);
		void deleteHash(string key);
		void resize();
	private:
		vector<list<dic<T>>> table;
		int size;//size of table
		int preHashing(string key);
		int hashing(string key);
	};
	//*****			template hash table			*****//
	//*****			translation string			*****//
	//don't know why but if the class isn't template, the definition should be inline
	class TString{
	public:
		TString(){}
	};
	//*****			translation string			*****//
};

#include "nstdlib.cpp"
#endif