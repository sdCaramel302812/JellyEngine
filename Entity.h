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

class AABB {//等軸包圍盒
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
	NO_COLLIDE = 0,		//無碰撞
	TRIANGLE,			//三角形
	RECTANGLE,			//矩形			//僅作為靜態物體使用
	CIRCLE,				//圓形
	GROUND_COLLIDER,				//網格
	CUBE,				//立方體
	SPHERE,				//球
	CYLINDER,			//圓柱
	LINE,				//線
	EVENT_COLLIDER,		//事件偵測器
};

enum EntityType {
	NORMAL,
	PLAYER,
	NPC,
	ENEMY,
	BULLET,
	BACK_GROUND,		//只存取背景圖，不碰撞
	GROUND,				//只包含地板碰撞箱，不繪製
	WALL,				//只包含牆壁碰撞箱，不繪製
};

class DynamicData {
public:
	//速度
	glm::vec3 velocity;
	//位置
	glm::vec3 position;

};

//如果超出 AABB 樹的容許值則重新插入




class RigidBody {
public:
	RigidBody(RigidType type = RECTANGLE);

	void setPosition(int x, int y, int z);
	void changePosition(float x, float y, float z);
	void setPosition(glm::vec3 pos);
	void changePosition(glm::vec3 changePos);
	
	//多面體頂點
	void setVertex(std::vector<glm::vec3> value);
	std::vector<glm::vec3> getVertices();
	std::vector<glm::vec4> getNormal();
	float getNormalLengthInverse();

	//圓形、球體半徑
	void setRadius(float r);
	float getRadius();

	void setAABB();
	AABB *getAABB();

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
	void setNormal();

	std::vector<glm::vec3> _vertices;//矩形頂點請順時鐘輸入
	std::vector<glm::vec4> _normal;  //面法線
	float _normal_length_inverse;
	float _radius;
	AABB bounding_box;
	
};


class Entity
{
public:
	Entity(int size = 0);
	~Entity();

	//碰撞箱
	RigidBody rigid;
	//繪圖相關 key
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
	int vertex_size;	//繪製時會用到(頂點數量)
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

