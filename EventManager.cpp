#include "EventManager.h"

queue<UpdateEvent<Entity *> *> EventManager::update_event;
queue<RenderEvent<Entity *> *> EventManager::render_event;

EventManager::EventManager()
{
}


EventManager::~EventManager()
{
}
