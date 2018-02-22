#pragma once
#include "ObjectManager.h"
#include "Button.h"
#include "TextItem.h"
#include "LevelEditorUI.h"
#include "Camera.h"

extern Camera camera;

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
	Button *_background_button;

	void drawBaseLine();

	glm::vec2 mouse2map(float x, float y);

private:
	bool _visable = true;
};

