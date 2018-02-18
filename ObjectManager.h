#pragma once
#include <vector>
#include "Entity.h"
#include <utility>
#include "nstdlib.h"
#include "AABBTree.h"
#include "UI.h"

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	//		Entity
	static void addEntity(Entity *);
	static void removeEntity(Entity *);
	static std::vector<Entity *> object_list;
	static AABBTree aabb_tree;
	//		Entity

	//		UI
	static std::vector<UI *> &getUI();
	static void addUI(UI *ui);
	static void removeUI(UI *ui);
	static void insertionSortForUI(std::vector<UI *> &list);
	//		UI



	static void qSort(std::vector<Entity *> list, bool _is_x);
	static void insertionSort(std::vector<std::pair<Entity *, glm::vec2>>, std::vector<Entity *> &list, bool _is_x);
	

private:
	static std::vector<UI *> ui_list;
};

