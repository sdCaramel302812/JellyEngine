#include "Entity.h"
#include <iostream>
using namespace std;

#define CONTAIN_LIMIT 0.1

AABB::AABB()
{
	_left_down_back = glm::vec3(0.0f, 0.0f, 0.0f);
	_right_top_front = glm::vec3(0.0f, 0.0f, 0.0f);
}

void AABB::unionAABB(AABB ab1)
{
	float left, right, top, down, front, back;
	left = ab1._left_down_back.x > _left_down_back.x ? _left_down_back.x : ab1._left_down_back.x;
	down = ab1._left_down_back.y > _left_down_back.y ? _left_down_back.y : ab1._left_down_back.y;
	right = ab1._right_top_front.x > _right_top_front.x ? ab1._right_top_front.x : _right_top_front.x;
	top = ab1._right_top_front.y > _right_top_front.y ? ab1._right_top_front.y : _right_top_front.y;
	front = ab1._right_top_front.z > _right_top_front.z ? ab1._right_top_front.z : _right_top_front.z;
	back = ab1._right_top_front.z < _right_top_front.z ? ab1._right_top_front.z : _right_top_front.z;
	_left_down_back.x = left;
	_left_down_back.y = down;
	_right_top_front.x = right;
	_right_top_front.y = top;
	_right_top_front.z = front;
	_left_down_back.z = back;
}

bool AABB::contain(AABB ab1)
{//檢查此 AABB 有無包含參數 ab1
	if (ab1._left_down_back.x < _left_down_back.x - CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._left_down_back.y < _left_down_back.y - CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._left_down_back.z < _left_down_back.z - CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._right_top_front.x > _right_top_front.x + CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._right_top_front.y > _right_top_front.y + CONTAIN_LIMIT) {
		return false;
	}
	if (ab1._right_top_front.z > _right_top_front.z + CONTAIN_LIMIT) {
		return false;
	}
	return true;
}

bool AABB::isCollided(AABB ab1)
{
	if ((ab1._left_down_back.x<_right_top_front.x && ab1._right_top_front.x>_left_down_back.x) || (ab1._left_down_back.x>_right_top_front.x && ab1._right_top_front.x<_left_down_back.x)) {
		if ((ab1._left_down_back.y<_right_top_front.y && ab1._right_top_front.y>_left_down_back.y) || (ab1._left_down_back.y>_right_top_front.y && ab1._right_top_front.y<_left_down_back.y)) {
			if ((ab1._left_down_back.z<_right_top_front.z && ab1._right_top_front.z>_left_down_back.z) || (ab1._left_down_back.z>_right_top_front.z && ab1._right_top_front.z<_left_down_back.z)) {
				return true;
			}
		}
	}
	return false;
}

float AABB::volume()
{
	return (_right_top_front.x - _left_down_back.x)*(_right_top_front.y - _left_down_back.y)*(_right_top_front.z - _left_down_back.z);
}

Entity::Entity(int size)
{
	vertex_size = size;
	_id = createID();
	e_type = NORMAL;
	_trigger = false;
	_visible = true;
}


Entity::~Entity()
{
}

void Entity::setSize(int s)
{
	vertex_size = s;
}

int Entity::size()
{
	return vertex_size;
}

bool Entity::isVisible()
{
	return _visible;
}

void Entity::setTrigger(bool value)
{
	_trigger = value;
}

bool Entity::isTrigger()
{
	return _trigger;
}

void Entity::trigger()
{
}

int Entity::_obj_num = 0;

int Entity::createID()
{
	_obj_num++;
	return _obj_num;
}



RigidBody::RigidBody(RigidType type)
{
	this->type = type;
}

void RigidBody::setVertex(std::vector<glm::vec3> value)
{
	_vertices = value;
	setNormal();
}

void RigidBody::setRadius(float r)
{
	_radius = r;
}

void RigidBody::setPosition(int x, int y, int z)
{//在此重設AABB
	data.position.x = x;
	data.position.y = y;
	data.position.z = z;
	setAABB();
}

void RigidBody::changePosition(float x, float y, float z)
{
	data.position.x += x;
	data.position.y += y;
	data.position.z += z;
	setAABB();
}

void RigidBody::setPosition(glm::vec3 pos)
{
	data.position.x = pos.x;
	data.position.y = pos.y;
	data.position.z = pos.z;
	setAABB();
}

void RigidBody::changePosition(glm::vec3 changePos)
{
	data.position.x += changePos.x;
	data.position.y += changePos.y;
	data.position.z += changePos.z;
	setAABB();
}

void RigidBody::setAABB()
{
	if (type == SPHERE) {
		bounding_box._left_down_back.x = data.position.x - _radius-0.1;
		bounding_box._left_down_back.y = data.position.y - _radius-0.1;
		bounding_box._right_top_front.x = data.position.x + _radius+0.1;
		bounding_box._right_top_front.y = data.position.y + _radius+0.1;
		bounding_box._left_down_back.z = data.position.z - _radius-0.1;
		bounding_box._right_top_front.z = data.position.z + _radius+0.1;
	}
	if (type == RECTANGLE) {
		float left = _vertices.at(0).x, right = _vertices.at(0).x, top = _vertices.at(0).y, down = _vertices.at(0).y, front = _vertices.at(0).z, back = _vertices.at(0).z;
		for (int i = 1; i < _vertices.size(); ++i) {
			if (_vertices.at(i).x < left) {
				left = _vertices.at(i).x;
			}
			if (_vertices.at(i).x > right) {
				right = _vertices.at(i).x;
			}
			if (_vertices.at(i).y < down) {
				down = _vertices.at(i).y;
			}
			if (_vertices.at(i).y > top) {
				top = _vertices.at(i).y;
			}
			if (_vertices.at(i).z < back) {
				back = _vertices.at(i).z;
			}
			if (_vertices.at(i).z > front) {
			front = _vertices.at(i).z;
			}
			bounding_box._left_down_back.x = left-0.1;
			bounding_box._left_down_back.y = down-0.1;
			bounding_box._right_top_front.x = right-0.1;
			bounding_box._right_top_front.y = top+0.1;
			bounding_box._left_down_back.z = back+0.1;
			bounding_box._right_top_front.z = front+0.1;
		}
	}
}

AABB * RigidBody::getAABB()
{
	return &bounding_box;
}

void RigidBody::setNormal()
{
	if (type == RECTANGLE) {
		glm::vec3 dir1;
		glm::vec3 dir2;
		glm::vec3 normal;

		dir1 = getVertices().at(0) - getVertices().at(1);
		dir2 = getVertices().at(1) - getVertices().at(2);
		normal = glm::cross(dir1, dir2);
		normal = glm::normalize(normal);
		_normal.push_back(normal);
	}
}

float RigidBody::getRadius()
{
	return _radius;
}

std::vector<glm::vec3> RigidBody::getVertices()
{
	return _vertices;
}

std::vector<glm::vec3> RigidBody::getNormal()
{
	return _normal;
}

SphereObject::SphereObject(int size) : Entity(size)
{
}

GroundObject::GroundObject(int size) : Entity(size)
{
}

Player::Player(int size) : Entity(size)
{
}




