#pragma once
#include "ObjectManager.h"
#include "Button.h"
#include "TextItem.h"

class LevelEditor
{
public:
	LevelEditor();
	~LevelEditor();

	void setVisable(bool value);
	bool visable();

	UI *current_UI = nullptr;
	Button *_save_button;
	Button *_load_button;
	Button *_wall_button;
	Button *_stair_button;
	Button *_ground_button;
	Button *_background_button;
	Button *_up_floor_button;
	Button *_down_floor_button;
	TextItem *_floor_text;


private:
	bool _visable = true;
};

