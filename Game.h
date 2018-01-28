#pragma once
#include <queue>
#include <vector>
#include "Event.h"
//#include "Event.cpp"
#include "Entity.h"
#include "Physics.h"
#include "EventManager.h"
#include "ObjectManager.h"
using std::queue;


static void foo();

enum GameState {
	PAUSE,
	MENU,
	ACTIVATE
};

enum CameraType {
	FP_CAMERA,
	TP_CAMERA,
	CAMERA_2D,
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
	void render();
	void update(float dt);
	void input();
};

