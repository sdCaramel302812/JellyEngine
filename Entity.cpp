#include "Entity.h"



Entity::Entity(int size)
{
	vertex_size = size;
	_id = createID();
	e_type = NORMAL;
	_trigger = false;
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

void RigidBody::setTriangleVertex(std::vector<glm::vec3> value)
{
}

void RigidBody::setRectangleVertex(std::vector<glm::vec3> value)
{
	vertices = value;
}

void RigidBody::setCircleRadius(float r)
{
}

void RigidBody::setGroundVertex(std::vector<glm::vec3> value)
{
}

void RigidBody::setCubeVertex(std::vector<glm::vec3> value)
{
}

void RigidBody::setSphereRadius(float r)
{
	radius = r;
}

void RigidBody::setPosition(int x, int y, int z)
{//在此重設AABB
}

void RigidBody::setAABB(int x1, int x2, int y1, int y2, int z1, int z2)
{
}

AABB & RigidBody::getAABB()
{
	// TODO: 於此處插入傳回陳述式
	return bounding_box;
}

float RigidBody::getRadius()
{
	return radius;
}

std::vector<glm::vec3> RigidBody::getVertices()
{
	return vertices;
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
