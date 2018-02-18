#pragma once
#include <queue>
#include <vector>
#include "Event.h"
//#include "Event.cpp"
#include "Entity.h"
#include "Physics.h"
#include "EventManager.h"
#include "ObjectManager.h"
#include "TextItem.h"
#include "ResourceManager.h"
#include "Scene.h"
using std::queue;


static void foo();

enum GameState {
	PAUSE,
	MENU,
	ACTIVATE,
	LEVEL_EDITOR,
};

class Game
{
public:
	GameState state;

	//Á{®É
	std::vector<Entity *> list;
	//

	Game();
	~Game();
	void Init();
	void textInit();
	void textureInit();
	void uiInit();

	void render();
	void update(float dt);
	void input(Scene &sc, float dt);
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	//test for mouse button callback
	void mouseButtonCallback(bool mouse_state);

private:
	double _mouse_x;
	double _mouse_y;
};

