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
#include "LevelEditor.h"
#include "LevelEditorUI.h"
#include "Define.h"
#include "ScrollList.h"
using std::queue;


static void foo();

enum GameState {
	PAUSE,
	MENU,
	ACTIVATE,
	LEVEL_EDITOR,
};

typedef std::function<void(void)> CALL_BACK;

class Game
{
public:
	GameState state;

	

	Game();
	~Game();
	void Init();
	void textInit();
	void textureInit();
	void uiInit();

	void gameLoop();

	void render();
	void update(float dt);
	void input(Scene &sc, float dt);
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	bool scrollCallback(GLFWwindow* window, double xoffset, double yoffset);		//回傳是否有執行 wheel event

	void textInput(Scene &sc, TString &text);
	void textInput(Scene &sc, string &text);


	LevelEditor *level_editor;
	Scene *scene;
	Player *player;

	std::map<string, CALL_BACK> &getPageMouseFunction();
	std::map<string, CALL_BACK> &getPageMouseClickFunction();
	std::map<string, CALL_BACK> &getPageKeyFunction();
	std::map<string, CALL_BACK> &getPageScrollFunction();
	void setPageMouseFunction(string key, CALL_BACK func);
	void setPageMouseClickFunction(string key, CALL_BACK func);
	void setPageKeyFunction(string key, CALL_BACK func);
	void setPageScrollFunction(string key, CALL_BACK func);
	void setCurrentPage(string key);

private:
	double _mouse_x;
	double _mouse_y;
	float dt;
	float last_frame;

	bool _is_first_click;
	int _last_press = clock();

	string _current_page;
	std::map<string, CALL_BACK> _page_mouse_function;
	std::map<string, CALL_BACK> _page_mouse_click_function;
	std::map<string, CALL_BACK> _page_key_function;
	std::map<string, CALL_BACK> _page_scroll_function;
};

