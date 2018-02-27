﻿#include "LevelEditor.h"
#include "Render.h"
using nstd::TString;



LevelEditor::LevelEditor()
{
	_save_button = new Button(100, 900, 240, 80);
	_load_button = new Button(400, 900, 240, 80);
	_wall_button = new Button(100, 500, 240, 80);
	_background_button = new Button(700, 900, 240, 80);
	_background_option_button = new Button(1000, 900, 240, 80);
	_background_list = new ScrollList(700, 500, 240, 400);
	_background_list->setItemHeight(80.0f);

	_save_button->setButtonText(TString("save"), _font_size);
	_load_button->setButtonText(TString("load"), _font_size);
	_wall_button->setButtonText(TString("wall"), _font_size);
	_background_button->setButtonText(TString("background"), _font_size);
	_background_option_button->setButtonText(TString(""), _font_size);

	_wall_button->setCallback([=]() {
		state = SET_WALL;
		cout << "set wall" << endl;
	});

	_background_button->setCallback([=]() {
		state = SET_BACKGROUND;
		_background_list->setVisable(true);
		cout << "set background" << endl;
	});

	_background_option_button->setCallback([=]() {
		if (state == SET_NOTHING) {
			state = SET_BACKGROUND_SIZE;
			_background_option_button->setButtonText(TString(L"set size"), _font_size);
			return;
		}
		if (state == SET_BACKGROUND_SIZE) {
			state = DELETE_BACKGROUND;
			_background_option_button->setButtonText(TString("delete"), _font_size);
			return;
		}
		if (state == DELETE_BACKGROUND) {
			state = SET_NOTHING;
			_background_option_button->setButtonText(TString(""), _font_size);
			return;
		}
	});

	

	ObjectManager::addUI(_save_button);
	ObjectManager::addText(_save_button->button_text);
	ObjectManager::addUI(_load_button);
	ObjectManager::addText(_load_button->button_text);
	ObjectManager::addUI(_wall_button);
	ObjectManager::addText(_wall_button->button_text);
	ObjectManager::addUI(_background_button);
	ObjectManager::addText(_background_button->button_text);
	ObjectManager::addUI(_background_option_button);
	ObjectManager::addText(_background_option_button->button_text);
	ObjectManager::addUI(_background_list);
	
}


LevelEditor::~LevelEditor()
{
}

void LevelEditor::setVisable(bool value)
{
	_visable = value;
}

bool LevelEditor::visable()
{
	return _visable;
}

void LevelEditor::drawBaseLine()
{
	Render::drawLine(glm::vec3(100, 0, 0), 1080, 90);
	Render::drawLine(glm::vec3(0, 100, 0), 1920, 0);
	float unit_length = 1080 / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z);
	Render::drawLine(glm::vec3(960 - unit_length / 2, 540, 0), unit_length, 0);
	Render::drawLine(glm::vec3(960, 540 - unit_length / 2, 0), unit_length, 90);

	float xpos = 960 * (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) / 1920 + camera.getOrthoIntervalX1X2Y1Y2().x;
	float ypos = 540 * (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) / 1080 + camera.getOrthoIntervalX1X2Y1Y2().z;
	glm::vec4 pos = glm::vec4(ypos, 0, xpos, 1);
	pos = glm::translate(glm::mat4(),camera.Position)*pos;
	if (state == SET_WALL) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "WALL", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	if (state == SET_BACKGROUND) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "BACKGROUND", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	else {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x), 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
}

void LevelEditor::setFontSize(int size)
{
	_font_size = size;
}

void LevelEditor::setBackgroundList()
{
	for (int i = 0; i < ResourceManager::getImageList().size(); ++i) {
		_background_list->addItem(ResourceManager::getImageList().at(i), [=]() {
			_current_texture = ResourceManager::getImageList().at(i);
			_background_list->setVisable(false);
		});
	}

	_background_list->setVisable(false);
}

glm::vec3 LevelEditor::mouse2map(float x, float y)	//將 UI 坐標系轉為世界坐標系，UI 坐標系的 X Y 分別對應世界坐標系的 Z X 
{
	float xpos = x * (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) / 1920 + camera.getOrthoIntervalX1X2Y1Y2().x;
	float ypos = y * (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) / 1080 + camera.getOrthoIntervalX1X2Y1Y2().z;
	glm::vec4 pos = glm::vec4(ypos, 0, xpos, 1);
	pos = glm::translate(glm::mat4(), camera.Position)*pos;
	pos.y -= 5;
	return pos;
}
