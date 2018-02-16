#pragma once
#include "Event.h"
#include "Entity.h"
#include <queue>
using std::queue;
using std::vector;

class EventManager
{
public:
	EventManager();
	~EventManager();

	static queue<UpdateEvent<Entity *> *> update_event;
	static queue<RenderEvent<Entity *> *> render_event;
	static vector<RenderEvent<Entity *> *> texture_render_event;
	static vector<RenderEvent<Entity *> *> color_render_event;

	//�N���P���谵�����A��� set uniform ����
	static void eventSort(vector<RenderEvent<Entity *> *> &list);
};

