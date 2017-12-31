#ifndef NSTDLIB_CPP
#define NSTDLIB_CPP
#include "nstdlib.h"


template<typename T>
nstd::dic<T>::dic(string Key, T Value) : key(Key), value(Value) {}

template<typename T>
nstd::hash<T>::hash(int m) : size(m)
{
	table.resize(m);
}

template<typename T>
void nstd::hash<T>::insert(dic<T> data)
{
	int index = hashing(data.key);
	table[index].push_back(data);
}

template<typename T>
T nstd::hash<T>::search(string key)
{
	int index = hashing(key);
	for (list<dic<T>>::iterator itr = table[index].begin(); itr != table[index].end(); ++itr) {
		if ((*itr).key == key) {
			return ((*itr).value);
		}
	}
}

template<typename T>
void nstd::hash<T>::deleteHash(string key)
{
	int index = hashing(key);
	for (list<dic>::iterator itr = table[index].begin(); itr != table[index].end(); ++itr) {
		if ((*itr).key == key) {
			table[index].erase(itr);
		}
	}
}

template<typename T>
void nstd::hash<T>::resize()
{}

template<typename T>
int nstd::hash<T>::preHashing(string key)
{
	int exp = 8, key_value = 0, p = 1;
	for (int i = key.size(); i > 0; --i) {
		key_value += key[i] * p;
		p *= exp;
	}
	return key_value;
}

template<typename T>
int nstd::hash<T>::hashing(string key)
{
	return preHashing(key) % size;
}



#endif

