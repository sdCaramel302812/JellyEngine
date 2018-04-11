#include "ObjectManager.h"
#include "Game.h"

extern Game *game;

std::vector<Entity *> ObjectManager::object_list;
AABBTree ObjectManager::aabb_tree;
std::vector<UI *> ObjectManager::ui_list;
std::vector<TextItem *> ObjectManager::text_list;


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::addEntity(Entity *obj)
{
	object_list.push_back(obj);
	aabb_tree.add(obj);
}

void ObjectManager::addPlayer(Player *obj)
{
	object_list.push_back(obj);
	aabb_tree.add(obj);
	game->player = obj;
	game->_has_player = true;
}

void ObjectManager::removeEntity(Entity *obj)
{
	aabb_tree.remove(obj->_aabb_node);
	for (std::vector<Entity *>::iterator itr = object_list.begin(); itr != object_list.end(); ++itr) {
		if (*itr == obj) {
			object_list.erase(itr);
			break;
		}
	}
	delete obj;
}

void ObjectManager::removePlayer(Entity *obj)
{
	aabb_tree.remove(obj->_aabb_node);
	for (std::vector<Entity *>::iterator itr = object_list.begin(); itr != object_list.end(); ++itr) {
		if (*itr == obj) {
			object_list.erase(itr);
			break;
		}
	}
	delete obj;
	game->_has_player = false;
}

void ObjectManager::textureSort(vector<Entity*>& list)
{
	//¨Ï¥Î±í±Æ§Ç
	std::map<string, std::vector<Entity*>> bucket;
	for (int i = 0; i < list.size(); ++i) {
		string texture = list.at(i)->texture;
		if (bucket.find(texture) == bucket.end()) {
			bucket.insert(std::pair<string, vector<Entity*>>());
		}
		bucket.at(texture).push_back(list.at(i));
	}
	vector<Entity*> new_list;
	for (std::map<string, std::vector<Entity*>>::const_iterator itr = bucket.begin(); itr != bucket.end(); ++itr) {
		for (int i = 0; i < itr->second.size(); ++i) {
			new_list.push_back(itr->second.at(i));
		}
	}
	list = new_list;
}

std::vector<UI*>& ObjectManager::getUI()
{
	return ui_list;
}

void ObjectManager::addUI(UI * ui)
{
	ui_list.push_back(ui);
	insertionSortForUI(ui_list);
}

void ObjectManager::removeUI(UI * ui)
{
	for (std::vector<UI *>::iterator itr = ui_list.begin(); itr != ui_list.end(); ++itr) {
		if (*itr == ui) {
			ui_list.erase(itr);
			break;
		}
	}
	delete ui;
}

void ObjectManager::qSort(std::vector<Entity*> list, bool _is_x)
{
	if (_is_x) {

	}
	else {

	}
}


void ObjectManager::insertionSortForUI(std::vector<UI*> &list)
{
	std::vector<UI *> sorted_list;
	sorted_list.resize(list.size());
		for (int i = 0; i < list.size(); ++i) {
			sorted_list.at(i) = list.at(i);
			for (int j = i; i > 0; --j) {
				if (sorted_list.at(j)->x() < sorted_list.at(j-1)->x()) {
					UI *tmp = sorted_list.at(j);
					sorted_list.at(j) = sorted_list.at(j - 1);
					sorted_list.at(j - 1) = tmp;
				}
				else {
					break;
				}
			}
		}
		list = sorted_list;
}

std::vector<TextItem*>& ObjectManager::getText()
{
	return text_list;
}

void ObjectManager::addText(TextItem * text_item)
{
	text_list.push_back(text_item);
}

void ObjectManager::removeText(TextItem * text_item)
{
}
