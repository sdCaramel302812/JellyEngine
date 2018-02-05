#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Math.h"
//#include "Render.h"
#include <string>
//#include "Event.h"

extern class AABBNode;

class AABB {//���b�]��
public:
	glm::vec3 _left_down_back;
	glm::vec3 _right_top_front;

	AABB();
	void unionAABB(AABB ab1);
	bool contain(AABB ab1);
	bool isCollided(AABB ab1);
	float volume();
};

enum RigidType {
	NO_COLLIDE = 0,		//�L�I��
	TRIANGLE,			//�T����
	RECTANGLE,			//�x��			//�ȧ@���R�A����ϥ�
	CIRCLE,				//���
	GROUND_COLLIDER,				//����
	CUBE,				//�ߤ���
	SPHERE,				//�y
	CYLINDER,			//��W
	LINE,				//�u
	EVENT_COLLIDER,		//�ƥ󰻴���
};

enum EntityType {
	NORMAL,
	PLAYER,
	NPC,
	ENEMY,
	BULLET,
	BACK_GROUND,		//�u�s���I���ϡA���I��
	GROUND,				//�u�]�t�a�O�I���c�A��ø�s
	WALL,				//�u�]�t����I���c�A��ø�s
};

class DynamicData {
public:
	//�t��
	glm::vec3 velocity;
	//��m
	glm::vec3 position;

};

//�p�G�W�X AABB �𪺮e�\�ȫh���s���J




class RigidBody {
public:
	RigidBody(RigidType type = RECTANGLE);

	void setPosition(int x, int y, int z);
	void changePosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);
	void changePosition(glm::vec3 changePos);
	
	//�h���鳻�I
	void setVertex(std::vector<glm::vec3> value);
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec4> getNormal();
	float getNormalLengthInverse();

	//��ΡB�y��b�|
	void setRadius(float r);
	float getRadius();

	void setAABB();
	AABB *getAABB();

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
	void setNormal();

	std::vector<glm::vec3> _vertices;//�x�γ��I�ж�������J
	std::vector<glm::vec4> _normal;  //���k�u
	float _normal_length_inverse;
	float _radius;
	AABB bounding_box;
	
};


class Entity
{
public:
	Entity(int size = 0);
	~Entity();

	//�I���c
	RigidBody rigid;
	//ø�Ϭ��� key
	std::string VAO;
	std::string VBO;
	std::string EBO;
	std::string texture;
	std::string specular;
	std::string shader;

	//set vertex size
	void setSize(int);
	int size();

	int _id;
	EntityType e_type;

	AABBNode *_aabb_node = nullptr;

	bool isVisible();
	void setTrigger(bool value);
	bool isTrigger();
	virtual void trigger();
private:
	int vertex_size;	//ø�s�ɷ|�Ψ�(���I�ƶq)
	bool _trigger;
	bool _visible;
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

