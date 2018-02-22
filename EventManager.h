#pragma once
#include "Event.h"
#include "Entity.h"
#include "UI.h"
#include <queue>
#include "TextItem.h"
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
	static vector<RenderEvent<UI *> *> draw_texture_ui_event;
	static vector<RenderEvent<UI *> *> draw_color_ui_event;
	static vector<RenderEvent<UI *> *> editor_ui_event;
	static vector<RenderEvent<TextItem *> *> draw_text_event;

	//將不同材質做分類，減少 set uniform 次數
	static void eventSort(vector<RenderEvent<Entity *> *> &list);
};

