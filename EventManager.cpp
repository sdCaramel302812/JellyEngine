#include "EventManager.h"
#include <map>

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

void EventManager::eventSort(vector<RenderEvent<Entity*>*> &list)
{
	//¨Ï¥Î±í±Æ§Ç
	std::map<string, std::vector<RenderEvent<Entity *>*>> bucket;
	for (int i = 0; i < list.size(); ++i) {
		string texture = list.at(i)->_target_id->texture;
		if (bucket.find(texture) == bucket.end()) {
			bucket.insert(std::pair<string, vector<RenderEvent<Entity *>*>>());
		}
		bucket.at(texture).push_back(list.at(i));
	}
	vector<RenderEvent<Entity*>*> new_list;
	for (std::map<string, std::vector<RenderEvent<Entity *>*>>::const_iterator itr = bucket.begin(); itr != bucket.end(); ++itr) {
		for (int i = 0; i < itr->second.size(); ++i) {
			new_list.push_back(itr->second.at(i));
		}
	}
	list = new_list;
}
