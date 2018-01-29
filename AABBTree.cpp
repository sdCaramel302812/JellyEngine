#include "AABBTree.h"
#include <queue>
#include <iostream>

AABBNode::AABBNode()
{
	_parent = nullptr;
	_children[0] = nullptr;
	_children[1] = nullptr;
	_data = nullptr;
}

bool AABBNode::isLeaf()
{
	return !(_data == nullptr);
}

void AABBNode::setBranch(AABBNode * n1, AABBNode * n2)
{
	n1->_parent = this;
	n2->_parent = this;
	_data = nullptr;
	_children[0] = n1;
	_children[1] = n2;
}

void AABBNode::setLeaf(Entity * e)
{
	_children[0] = nullptr;
	_children[1] = nullptr;
	_data = e;
	e->_aabb_node = this;
}

AABB unionAABB(AABB ab1, AABB ab2)
{
	float left, right, top, down, front, back;
	left = ab1._left_down_back.x > ab2._left_down_back.x ? ab2._left_down_back.x : ab1._left_down_back.x;
	down = ab1._left_down_back.y > ab2._left_down_back.y ? ab2._left_down_back.y : ab1._left_down_back.y;
	right = ab1._right_top_front.x > ab2._right_top_front.x ? ab1._right_top_front.x : ab2._right_top_front.x;
	top = ab1._right_top_front.y > ab2._right_top_front.y ? ab1._right_top_front.y : ab2._right_top_front.y;
	front = ab1._right_top_front.z > ab2._right_top_front.z ? ab1._right_top_front.z : ab2._right_top_front.z;
	back = ab1._left_down_back.z < ab2._left_down_back.z ? ab1._left_down_back.z : ab2._left_down_back.z;
	AABB result;
	result._left_down_back.x = left;
	result._left_down_back.y = down;
	result._right_top_front.x = right;
	result._right_top_front.y = top;
	result._left_down_back.z = back;
	result._right_top_front.z = front;
	return result;
}

void AABBNode::updateAABB(float margin)
{
	if (isLeaf()) {
		AABB tmp = *_data->rigid.getAABB();
		tmp._left_down_back -= margin;
		tmp._right_top_front += margin;
		_aabb = tmp;
	}
	else {
		AABB tmp = unionAABB(_children[0]->_aabb, _children[1]->_aabb);
		//tmp._left_down_back -= margin;
		//tmp._right_top_front += margin;
		_aabb = tmp;
	}
}

AABBNode * AABBNode::getSibling()
{
	if (_parent) {
		return ((this == _parent->_children[0]) ? _parent->_children[1] : _parent->_children[0]);
	}
}

AABBTree::AABBTree()
{
	margin = 0.5;
}

void AABBTree::setMargin(float m)
{
	margin = m;
}

void AABBTree::add(Entity *e)
{
	if (_root) {
		AABBNode *node = new AABBNode();
		node->setLeaf(e);
		node->updateAABB(margin);
		insert(node, &_root);
	}
	else {
		_root = new AABBNode();
		_root->setLeaf(e);
		_root->updateAABB(margin);
	}
}

void AABBTree::insert(AABBNode * node, AABBNode ** parent)
{	//�p�G�Oleaf�A�Nleaf ������branch�A�ñN���leaf ���W
	//�p�G�Obranch�A�Nnode ���J���n�ܤƸ��p�� sub tree
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

void AABBTree::remove(AABBNode *node)
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

void AABBTree::update()
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
			remove(invalid_node.back());
			invalid_node.pop_back();
			add(tmp);
		}
	}
}


void AABBTree::findInvalidNode(AABBNode * node)
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

void AABBTree::collidingHelperUp(AABBNode * node, AABBNode *up)
{
	AABBNode *sibling = up->getSibling();
	collidingHelper(node, sibling);
	if (up->_parent) {
		collidingHelperUp(node, up->_parent);
	}
}

void AABBTree::collidingHelper(AABBNode * node, AABBNode *sibling)
{
	if (node->_aabb.isCollided(sibling->_aabb)) {
		if (sibling->isLeaf()) {
			collision_list.push_back(sibling->_data);
		}
		else {
			collidingHelperDown(node, sibling);
		}
	}
}

void AABBTree::collidingHelperDown(AABBNode * node, AABBNode *cmp)
{
	if (node->_aabb.isCollided(cmp->_children[0]->_aabb)) {
		if (cmp->_children[0]->isLeaf()) {
			collision_list.push_back(cmp->_children[0]->_data);
		}
		else {
			collidingHelperDown(node, cmp->_children[0]);
		}
	}
	if (node->_aabb.isCollided(cmp->_children[1]->_aabb)) {
		if (cmp->_children[1]->isLeaf()) {
			collision_list.push_back(cmp->_children[1]->_data);
		}
		else {
			collidingHelperDown(node, cmp->_children[1]);
		}
	}
}

//�V�W�ˬd parent �� sibling �A�Y�ۥ�h�V�U�ˬd
std::vector<Entity*>& AABBTree::collisionWith(AABBNode *node)
{
	collision_list.clear();
	if (!_root || _root->isLeaf()) {
		return collision_list;
	}
	collidingHelperUp(node, node);

	return collision_list;
}