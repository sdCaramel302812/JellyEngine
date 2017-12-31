#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include "Render.h"
#include <string>
//#include "Event.h"


enum RigidType {
	NO_COLLIDE = 0,		//�L�I��
	TRIANGLE,	//�T����
	RECTANGLE,	//�x��
	CIRCLE,		//���
	GROUND,		//����
	CUBE,		//�ߤ���
	SPHERE,		//�y
	CYLINDER,	//��W
	LINE		//�u
};

enum EntityType {
	NORMAL,
	PLAYER,
};

class DynamicData {
public:
	//�t��
	glm::vec3 velocity;
	//��m
	glm::vec3 position;

};

class AABB {//���b�]��
public:
	int _x_top;
	int _x_bottom;
	int _y_top;
	int _y_bottom;
	int _z_top;
	int _z_bottom;
};

class RigidBody {
public:
	RigidBody(RigidType type = RECTANGLE);

	void setTriangleVertex(std::vector<glm::vec3> value);
	void setRectangleVertex(std::vector<glm::vec3> value);
	void setCircleRadius(float r);
	void setGroundVertex(std::vector<glm::vec3> value);
	void setCubeVertex(std::vector<glm::vec3> value);
	void setSphereRadius(float r);
	void setPosition(int x, int y, int z);
	void setAABB(int x1,int x2, int y1, int y2, int z1, int z2);
	AABB &getAABB();
	float getRadius();
	std::vector<glm::vec3> getVertices();

	DynamicData data;

	//���\���z�ĪG
	bool _allow_physics = true;
	//�ण�ಾ��
	bool _is_static = false;
	//��q
	float _mass = 1;
	//��_�Y�ơA0�������D�u�I�A1���u�I
	float _restitution_coeffient = 1;
	//�����Y��
	float _friction_coeffient = 3;
	RigidType type;
private:
	std::vector<glm::vec3> vertices;//�x�γ��I�ж�������J
	float radius;
	AABB bounding_box;

};


class Entity
{
public:
	Entity(int size = 0);
	~Entity();

	//�I���c
	RigidBody rigid;
	std::string VAO;
	std::string VBO;
	std::string EBO;
	std::string texture;
	std::string specular;
	std::string shader;
	void setSize(int);
	int size();
	int _id;
	EntityType e_type;
	void setTrigger(bool value);
	bool isTrigger();
	virtual void trigger();
private:
	int vertex_size;	//ø�s�ɷ|�Ψ�
	bool _trigger;
	static int createID();
	static int _obj_num;

	
};

class Player :public Entity {
public:
	Player(int size = 0);
};

class SphereObject :public Entity {
public:
	SphereObject(int size = 0);
};

class GroundObject :public Entity {
public:
	GroundObject(int size = 0);
};
