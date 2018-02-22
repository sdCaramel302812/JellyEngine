#include "LevelEditor.h"
#include "Render.h"
using nstd::TString;



LevelEditor::LevelEditor()
{
	_save_button = new Button(100, 900, 300, 120);
	_load_button = new Button(500, 900, 300, 120);
	_wall_button = new Button(100, 500, 300, 120);
	_background_button = new Button(900, 900, 300, 120);

	_save_button->setButtonText(TString("save"));
	_load_button->setButtonText(TString("load"));
	_wall_button->setButtonText(TString("wall"));
	_background_button->setButtonText(TString("background"));

	ObjectManager::addUI(_save_button);
	ObjectManager::addText(_save_button->button_text);
	ObjectManager::addUI(_load_button);
	ObjectManager::addText(_load_button->button_text);
	ObjectManager::addUI(_wall_button);
	ObjectManager::addText(_wall_button->button_text);
	ObjectManager::addUI(_background_button);
	ObjectManager::addText(_background_button->button_text);
	
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
	Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x), 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
}

glm::vec2 LevelEditor::mouse2map(float x, float y)
{
	float xpos = x * (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) / 1920 + camera.getOrthoIntervalX1X2Y1Y2().x;
	float ypos = y * (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) / 1080 + camera.getOrthoIntervalX1X2Y1Y2().z;
	glm::vec4 pos = glm::vec4(ypos, 0, xpos, 1);
	pos = glm::translate(glm::mat4(), camera.Position)*pos;
	return glm::vec2(pos.x, pos.z);
}
