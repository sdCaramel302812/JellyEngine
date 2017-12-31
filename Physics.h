#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Entity.h"
#include "Event.h"
#include "ObjectManager.h"
#include "EventManager.h"

class Physics
{
public:
	//all the function should be static
	static void displace(Entity *obj, float dt);
	static int collisionDetect(Entity *A, Entity *B, MotionEvent<Entity *> *event);
	Physics();
	~Physics();
private:
	const static float g;//重力加速度
};

