#pragma once
#include "Event.h"
#include "Entity.h"
#include <queue>
using std::queue;

class EventManager
{
public:
	EventManager();
	~EventManager();

	static queue<UpdateEvent<Entity *> *> update_event;
	static queue<RenderEvent<Entity *> *> render_event;
};

