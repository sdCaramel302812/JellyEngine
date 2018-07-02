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
typedef std::function<void(GLFWwindow *, double, double)> MOUSE_CALL_BACK;
typedef std::function<void(GLFWwindow *, int, int, int)> MOUSE_BUTTON_CALL_BACK;
typedef std::function<bool(GLFWwindow *, double, double)> SCROLL_CALL_BACK;
typedef std::function<void(Scene &, float)> KEY_CALL_BACK;

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
	bool _has_player = false;

	std::map<string, MOUSE_CALL_BACK> &getPageMouseFunction();
	std::map<string, MOUSE_BUTTON_CALL_BACK> &getPageMouseClickFunction();
	std::map<string, KEY_CALL_BACK> &getPageKeyFunction();
	std::map<string, SCROLL_CALL_BACK> &getPageScrollFunction();
	void setPageMouseFunction(string key, MOUSE_CALL_BACK func);
	void setPageMouseClickFunction(string key, MOUSE_BUTTON_CALL_BACK func);
	void setPageKeyFunction(string key, KEY_CALL_BACK func);
	void setPageScrollFunction(string key, SCROLL_CALL_BACK func);
	void setCurrentPage(string key);

	//			level editor function
	void EditorKeyCallback(Scene &sc, float dt);
	void EditorMouseCallback(GLFWwindow* window, double xpos, double ypos);
	void EditorMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	bool EditorScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	//

	//			default function
	void DefaultKeyCallback(Scene &sc, float dt);
	void DefaultMouseCallback(GLFWwindow* window, double xpos, double ypos);
	void DefaultMouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
	bool DefaultScrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	//

	float _time_scale = 1;
protected:
	double _mouse_x;
	double _mouse_y;
	float dt;
	float last_frame;

	bool _debug_mode = true;
	bool _is_first_click;
	int _last_press = clock();

	string _current_page;
	std::map<string, MOUSE_CALL_BACK> _page_mouse_function;
	std::map<string, MOUSE_BUTTON_CALL_BACK> _page_mouse_click_function;
	std::map<string, KEY_CALL_BACK> _page_key_function;
	std::map<string, SCROLL_CALL_BACK> _page_scroll_function;

	// for 3d debug
	double _last_x;
	double _last_y;
	bool _first_mouse = true;
};

