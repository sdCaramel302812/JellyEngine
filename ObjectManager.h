#pragma once
#include <vector>
#include "Entity.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	static std::vector<Entity *> object_list;
};

