#include "ObjectManager.h"


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
}

void ObjectManager::qSort(std::vector<Entity*> list, bool _is_x)
{
	if (_is_x) {

	}
	else {

	}
}

void ObjectManager::insertionSort(std::vector<std::pair<Entity *, glm::vec2>> unsort_list, std::vector<Entity*> &list, bool _is_x)
{
	std::vector<Entity *> sorted_list;
	sorted_list.resize(unsort_list.size());
	if (_is_x) {
		for (int i = 0; i < list.size(); ++i) {
			sorted_list.at(i) = unsort_list.at(i).first;
			for (int j = i; i > 0; --j) {
				if (sorted_list.at(j)->rigid.getAABB()->_left_down_back.x < sorted_list.at(j - 1)->rigid.getAABB()->_left_down_back.x) {
					Entity *tmp = sorted_list.at(j);
					sorted_list.at(j) = sorted_list.at(i);
					sorted_list.at(i) = tmp;
					unsort_list.at(i).second.x--;//not correct
				}
				else {
					break;
				}
			}
		}
	}
	else {
		for (int i = 0; i < unsort_list.size(); ++i) {
			sorted_list.at(i) = unsort_list.at(i).first;
			for (int j = i; i > 0; --j) {
				if (sorted_list.at(j)->rigid.getAABB()->_left_down_back.y < sorted_list.at(j - 1)->rigid.getAABB()->_left_down_back.y) {
					Entity *tmp = sorted_list.at(j);
					sorted_list.at(j) = sorted_list.at(i);
					sorted_list.at(i) = tmp;
					unsort_list.at(i).second.y--;
				}
				else {
					break;
				}
			}
		}
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
