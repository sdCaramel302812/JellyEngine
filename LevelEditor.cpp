#include "LevelEditor.h"
using nstd::TString;



LevelEditor::LevelEditor()
{
	_save_button = new Button(100, 900, 300, 120);
	_load_button = new Button(500, 900, 300, 120);
	_wall_button = new Button(100, 500, 300, 120);
	_stair_button = new Button(100, 350, 300, 120);
	_ground_button = new Button(100, 200, 300, 120);
	_background_button = new Button(900, 900, 300, 120);
	_up_floor_button = new Button(1600, 600, 300, 120);
	_down_floor_button = new Button(1600, 300, 300, 120);
	_floor_text = new TextItem(1620, 480, 300, 120);

	_save_button->setButtonText(TString("save"));
	_load_button->setButtonText(TString("load"));
	_wall_button->setButtonText(TString("wall"));
	_stair_button->setButtonText(TString("stair"));
	_ground_button->setButtonText(TString("ground"));
	_background_button->setButtonText(TString("background"));
	_up_floor_button->setButtonText(TString("up floor"));
	_down_floor_button->setButtonText(TString("down floor"));
	_floor_text->setText(TString("1 F"));

	ObjectManager::addUI(_save_button);
	ObjectManager::addUI(_load_button);
	ObjectManager::addUI(_wall_button);
	ObjectManager::addUI(_stair_button);
	ObjectManager::addUI(_background_button);
	ObjectManager::addUI(_ground_button);
	ObjectManager::addUI(_up_floor_button);
	ObjectManager::addUI(_down_floor_button);
	ObjectManager::addUI(_floor_text);

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
