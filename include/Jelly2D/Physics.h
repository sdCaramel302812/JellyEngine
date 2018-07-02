#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Entity.h"
#include "nstdlib.h"
#include "Event.h"
#include "ObjectManager.h"
#include "EventManager.h"

class simplex3D {

};

class Physics
{
public:
	//all the function should be static
	static void displace(Entity *obj, float dt);
	static int collisionDetect(Entity *A, Entity *B, MotionEvent<Entity *> *event);
	//this function is using to do View Frustum Culling 
	static bool GJK(std::vector<glm::vec3> &vertices1, std::vector<glm::vec3> &vertices2, float radius = 0);
	static glm::vec3 GJKSupport(std::vector<glm::vec3> &vertices, std::vector<bool> &has_find, glm::vec3 dir, bool &end);
	static bool GJKSupportRect(std::vector<glm::vec3> vertices, std::vector<bool> has_find, glm::vec3 dir, bool &end);
	//fast inverse square root
	//please don't put integer into this function
	static float qsqrt(float number);
	Physics();
	~Physics();
private:
	const static float g;//重力加速度
};

