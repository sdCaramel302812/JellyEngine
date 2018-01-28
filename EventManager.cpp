#include "EventManager.h"

queue<UpdateEvent<Entity *> *> EventManager::update_event;
queue<RenderEvent<Entity *> *> EventManager::render_event;
vector<RenderEvent<Entity *> *> EventManager::color_render_event;
vector<RenderEvent<Entity *> *> EventManager::texture_render_event;

EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}

void EventManager::eventSort(vector<RenderEvent<Entity*>*>)
{
}
