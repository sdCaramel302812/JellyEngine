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
/*


nstd::AABBNode::AABBNode()
{
	_parent = nullptr;
	_children[0] = nullptr;
	_children[1] = nullptr;
	_data = nullptr;
}

bool nstd::AABBNode::isLeaf()
{
	return !(_data == nullptr);
}

void nstd::AABBNode::setBranch(AABBNode * n1, AABBNode * n2)
{
	n1->_parent = this;
	n2->_parent = this;
	_data = nullptr;
	_id = -1;
	_children[0] = n1;
	_children[1] = n2;
}

void nstd::AABBNode::setLeaf(Entity * e, int id)
{
	_children[0] = nullptr;
	_children[1] = nullptr;
	_data = e;
	e->_aabb_node = this;
	_id = id;
}

AABB nstd::unionAABB(AABB ab1, AABB ab2)
{
	float left, right, top, down, front, back;
	left = ab1._left_down_back.x > ab2._left_down_back.x ? ab2._left_down_back.x : ab1._left_down_back.x;
	down = ab1._left_down_back.y > ab2._left_down_back.y ? ab2._left_down_back.y : ab1._left_down_back.y;
	right = ab1._right_top_front.x > ab2._right_top_front.x ? ab1._right_top_front.x : ab2._right_top_front.x;
	top = ab1._right_top_front.y > ab2._right_top_front.y ? ab1._right_top_front.y : ab2._right_top_front.y;
	front = ab1._right_top_front.z > ab2._right_top_front.z ? ab1._right_top_front.z : ab2._right_top_front.z;
	back = ab1._right_top_front.z < ab2._right_top_front.z ? ab1._right_top_front.z : ab2._right_top_front.z;
	AABB result;
	result._left_down_back.x = left;
	result._left_down_back.y = down;
	result._right_top_front.x = right;
	result._right_top_front.y = top;
	result._left_down_back.z = back;
	result._right_top_front.z = front;
	return result;
}

void nstd::AABBNode::updateAABB(float margin)
{
	if (isLeaf()) {
		AABB tmp = *_data->rigid.getAABB();
		tmp._left_down_back -= margin;
		tmp._right_top_front += margin;
		_aabb = tmp;
	}
	else {
		AABB tmp = unionAABB(_children[0]->_aabb, _children[1]->_aabb);
		tmp._left_down_back -= margin;
		tmp._right_top_front += margin;
		_aabb = tmp;
	}
}

nstd::AABBNode * nstd::AABBNode::getSibling()
{
	if (_parent) {
		return ((this == _parent->_children[0]) ? _parent->_children[1] : _parent->_children[0]);
	}
}

void nstd::AABBTree::add(Entity *e, int id)
{
	if (_root) {
		AABBNode *node = new AABBNode();
		node->setLeaf(e, id);
		node->updateAABB(margin);
		insert(node, &_root);
	}
	else {
		_root = new AABBNode();
		_root->setLeaf(e, id);
		_root->updateAABB(margin);
	}
}

void nstd::AABBTree::insert(AABBNode * node, AABBNode ** parent)
{	//如果是leaf，將leaf 替換成branch，並將兩個leaf 接上
	//如果是branch，將node 插入面積變化較小的 sub tree
	AABBNode *p = *parent;
	if (p->isLeaf()) {
		AABBNode *new_parent = new AABBNode();
		if (p->_parent) {
			new_parent->_parent = p->_parent;
		}
		p->_parent = new_parent;
		new_parent->setBranch(p, node);
		*parent = new_parent;
	}
	else {
		AABB aabb1 = unionAABB(p->_children[0]->_aabb, node->_aabb);
		AABB aabb2 = unionAABB(p->_children[1]->_aabb, node->_aabb);
		float diff1 = aabb1.volume() - p->_children[0]->_aabb.volume();
		float diff2 = aabb2.volume() - p->_children[0]->_aabb.volume();
		if (diff1 > diff2) {
			insert(node, &p->_children[1]);
		}
		else {
			insert(node, &p->_children[0]);
		}
	}
	(*parent)->updateAABB(margin);
}

void nstd::AABBTree::remove(AABBNode *node)
{
	AABBNode *parent = node->_parent;
	if (parent) {
		AABBNode *sibling = node->getSibling();
		if (parent->_parent) {
			sibling->_parent = parent->_parent;
			if (parent == parent->_parent->_children[0]) {
				parent->_parent->_children[0] = sibling;
			}
			else {
				parent->_parent->_children[1] = sibling;
			}
		}
		else {
			sibling->_parent = nullptr;
			_root = sibling;
		}
		delete parent;
		delete node;
	}
	else {
		_root = nullptr;
		delete node;
	}
}

void nstd::AABBTree::update()
{
	if (!_root) {
		return;
	}
	if (_root->isLeaf()) {
		_root->updateAABB(margin);
	}
	else {
		invalid_node.clear();
		findInvalidNode(_root);
		while (!invalid_node.empty()) {
			Entity *tmp = invalid_node.back()->_data;
			int id = invalid_node.back()->_id;
			remove(invalid_node.back());
			invalid_node.pop_back();
			add(tmp, id);
		}
	}
}

#include <iostream>

nstd::AABBNode * nstd::AABBTree::pick(AABB *ab1, int id)
{
	std::queue<AABBNode *> que;
	AABBNode *current = _root;
	que.push(current);
	while (!que.empty()) {
		current = que.front();
		if (!current->isLeaf()) {
			if (current->_children[0]->_aabb.contain(*ab1)) {
				que.push(current->_children[0]);
			}
			if (current->_children[1]->_aabb.contain(*ab1)) {
				que.push(current->_children[1]);
			}
		}
		else {
			//std::cout << current->_id << "\t"<< id << std::endl;
			if (current->_id == id) {
				return current;
			}
		}
		que.pop();
	}
	return nullptr;
}

void nstd::AABBTree::findInvalidNode(AABBNode * node)
{
	if (node->isLeaf()) {
		if (!node->_aabb.contain(*(node->_data->rigid.getAABB()))) {
			invalid_node.push_back(node);
		}
	}
	else {
		findInvalidNode(node->_children[0]);
		findInvalidNode(node->_children[1]);
	}
}

void nstd::AABBTree::collidingHelperUp(AABBNode * node)
{
	AABBNode *sibling = node->getSibling();
	//collidingHelper(node, sibling);
	if (node->_parent) {
	//	collidingHelperUp(node->_parent);
	}
}

void nstd::AABBTree::collidingHelper(AABBNode * node, AABBNode *sibling)
{
	if (node->_aabb.isCollided(sibling->_aabb)) {
		if (sibling->isLeaf()) {
			collision_list.push_back(sibling);
		}
		else {
			collidingHelper(node, sibling);
		}
	}
}

void nstd::AABBTree::collidingHelperDown(AABBNode * node, AABBNode *cmp)
{
	if (node->_aabb.isCollided(cmp->_children[0]->_aabb)) {
		if (cmp->_children[0]->isLeaf()) {
			collision_list.push_back(cmp->_children[0]);
		}
		else {
			collidingHelper(node, cmp->_children[0]);
		}
	}
	if (node->_aabb.isCollided(cmp->_children[1]->_aabb)) {
		if (cmp->_children[1]->isLeaf()) {
			collision_list.push_back(cmp->_children[1]);
		}
		else {
			collidingHelper(node, cmp->_children[1]);
		}
	}
}

//向上檢查 parent 的 sibling ，若相交則向下檢查
std::vector<nstd::AABBNode*>& nstd::AABBTree::collisionWith(AABBNode *node)
{
	collision_list.clear();
	if (!_root || _root->isLeaf()) {
		return collision_list;
	}
	collidingHelperUp(node);

	return collision_list;
}

*/