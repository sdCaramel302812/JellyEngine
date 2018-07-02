#pragma once
#include "ObjectManager.h"
#include "Button.h"
#include "TextItem.h"
#include "LevelEditorUI.h"
#include "Camera.h"
#include "Entity.h"
#include "ScrollList.h"
#include "ResourceManager.h"
#include <vector>
#include <functional>
using std::vector;

extern Camera camera;

enum EditorState {
	SET_NOTHING,
	SET_WALL,
	SET_BACKGROUND,
	SET_BACKGROUND_SIZE,
	DELETE_BACKGROUND,
	SET_FILE_NAME,
	SET_PREFAB,
	SET_PREFAB_POSITION,
	DELETE_PREFAB,
};


typedef std::function<void(string, glm::vec3)> PREFAB_FUNCTION;

class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();


	void setVisable(bool value);
	bool visable();
	
	string _current_prefab;					//prefab name
	string _current_texture;				//background texture
	string _map_name;						//the map that is edit now
	
	UI *current_UI = nullptr;				//when editing wall
	Entity *current_entity = nullptr;		//when editing background, prefab etc.
	Button *_save_button;
	Button *_load_button;
	Button *_wall_button;
	Button *_background_button;
	Button *_background_option_button;
	Button *_prefab_button;
	Button *_prefab_option_button;
	Button *_set_file_name_button;
	Button *_play_button;
	TextItem *_file_name;
	ScrollList *_background_list;
	ScrollList *_map_list;
	ScrollList *_prefab_list;				//<<--------------------------------------	不知道為甚麼就是不顯示


	PREFAB_FUNCTION _prefab_adder;
	bool _prefab_has_init = false;
	void initPrefabAdder(PREFAB_FUNCTION func);

	void drawBaseLine();					//not really useful
	void setFontSize(int size);				//font size in all editor elements

	void setPrefabList();					//setup scroll list
	void setBackgroundList();				//setup scroll list
	void setMapList();						//setup scroll list

	glm::vec3 mouse2map(float x, float y);	//coordinate transforms

	EditorState state = SET_NOTHING;

	bool _is_setting = false;				//is setting the position for map

private:
	bool _visable = true;
	int _font_size = 24;
};

