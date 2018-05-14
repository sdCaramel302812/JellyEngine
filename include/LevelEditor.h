#pragma once
#include "ObjectManager.h"
#include "Button.h"
#include "TextItem.h"
#include "LevelEditorUI.h"
#include "Camera.h"
#include "Entity.h"
#include "ScrollList.h"
#include "ResourceManager.h"

extern Camera camera;

enum EditorState {
	SET_NOTHING,
	SET_WALL,
	SET_BACKGROUND,
	SET_BACKGROUND_SIZE,
	DELETE_BACKGROUND,
	SET_FILE_NAME,
};


class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();

	void setVisable(bool value);
	bool visable();

	string _current_texture;
	string _map_name;

	UI *current_UI = nullptr;
	Entity *current_entity = nullptr;
	Button *_save_button;
	Button *_load_button;
	Button *_wall_button;
	Button *_background_button;
	Button *_background_option_button;
	Button *_set_file_name_button;
	Button *_play_button;
	TextItem *_file_name;
	ScrollList *_background_list;
	ScrollList *_map_list;

	void drawBaseLine();
	void setFontSize(int size);

	void setBackgroundList();
	void setMapList();

	glm::vec3 mouse2map(float x, float y);

	EditorState state = SET_NOTHING;

	bool _is_setting = false;	//is setting the position for map

private:
	bool _visable = true;
	int _font_size = 36;
};
