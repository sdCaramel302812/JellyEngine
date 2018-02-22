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


namespace nstd {
	//*****			template hash table			*****//
	template<typename T>
	class dic {
	public:
		string key;
		T value;
		dic(string Key, T Value) :key(Key), value(Value) {}
	};

	template<typename T>
	class hash {
	public:
		hash(int m = 100) :size(m){
			table.resize(m);
		}

		void insert(dic<T> data) {
			int index = hashing(data.key);
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
			for (list<dic>::iterator itr = table[index].begin(); itr != table[index].end(); ++itr) {
				if ((*itr).key == key) {
					table[index].erase(itr);
				}
			}
		}

		void resize() {}
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
		nstd::TString operator+(nstd::TString value);
		nstd::TString operator+(string value);
		nstd::TString operator+(wstring value);
		nstd::TString operator+(const char *value);
		int operator=(string value);
		int operator=(wstring value);
		wchar_t operator[](int x);
		nstd::TString &operator+=(nstd::TString value);
		nstd::TString &operator+=(string value);
		nstd::TString &operator+=(wstring value);
		nstd::TString &operator+=(const char *value);
		std::istream &operator>>(std::istream &is);
		std::ostream &operator<<(std::ostream &os);
		std::ifstream &operator>>(std::ifstream &is);
		std::ofstream &operator<<(std::ofstream &os);
		string wstr2str(std::wstring const & src);
		wstring str2wstr(std::string const & src);
		nstd::TString &number(double num);		//this function will let the data be the number
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
	//*****				AABB tree				*****//
	class AABBNode;
	/*
	

	AABB unionAABB(AABB ab1, AABB ab2);

	class AABBNode {
	public:
		AABBNode *_parent;
		AABBNode *_children[2];


		//node's bounding box
		AABB _aabb;
		//things that should be collide
		Entity *_data;
		AABBNode();
		bool isLeaf();
		void setBranch(AABBNode *n1, AABBNode *n2);
		void setLeaf(Entity * e, int id);
		void updateAABB(float margin);
		//sibling : ¥S§Ì
		AABBNode *getSibling();

		//Entity id
		int _id = -1;
	};
	
	class AABBTree {
	public:	
		void add(Entity *e, int id);
		void remove(AABBNode *node);
		void update();
		AABBNode *pick(AABB *ab1, int id);
		std::vector<AABBNode *> &collisionWith(AABBNode *);

		AABBNode *_root;
		float margin = 0.5;

	private:
		void insert(AABBNode *node, AABBNode **parent);
		void findInvalidNode(AABBNode *node);
		void collidingHelperDown(AABBNode *node, AABBNode *cmp);
		void collidingHelperUp(AABBNode *node);
		void collidingHelper(AABBNode *node, AABBNode *sibling);
		std::vector<AABBNode *> invalid_node;
		std::vector<AABBNode *> collision_list;
	};

	*/
	//*****				AABB tree				*****//
};

#endif