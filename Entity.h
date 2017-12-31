#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
//#include "Render.h"
#include <string>
//#include "Event.h"


enum RigidType {
	NO_COLLIDE = 0,		//無碰撞
	TRIANGLE,	//三角形
	RECTANGLE,	//矩形
	CIRCLE,		//圓形
	GROUND,		//網格
	CUBE,		//立方體
	SPHERE,		//球
	CYLINDER,	//圓柱
	LINE		//線
};

enum EntityType {
	NORMAL,
	PLAYER,
};

class DynamicData {
public:
	//速度
	glm::vec3 velocity;
	//位置
	glm::vec3 position;

};

class AABB {//等軸包圍盒
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

	//允許物理效果
	bool _allow_physics = true;
	//能不能移動
	bool _is_static = false;
	//質量
	float _mass = 1;
	//恢復係數，0為完全非彈碰，1為彈碰
	float _restitution_coeffient = 1;
	//摩擦係數
	float _friction_coeffient = 3;
	RigidType type;
private:
	std::vector<glm::vec3> vertices;//矩形頂點請順時鐘輸入
	float radius;
	AABB bounding_box;

};


class Entity
{
public:
	Entity(int size = 0);
	~Entity();

	//碰撞箱
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
	int vertex_size;	//繪製時會用到
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
