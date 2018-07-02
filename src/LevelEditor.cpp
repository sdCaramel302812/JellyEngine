#include "LevelEditor.h"
#include "Render.h"
#include "Game.h"
#include "Camera.h"

extern Game *game;
extern Camera camera;

LevelEditor::LevelEditor()
{
	//test	vvv
	//PrefabAdder *adder = new PrefabAdder();
	//Player *p = new Player(glm::vec3());
	//MovableObject *m =new MovableObject(glm::vec3());

	//test	^^^
	_save_button = new Button(100, 1030, 180, 50);
	_load_button = new Button(280, 1030, 180, 50);
	_wall_button = new Button(100, 500, 180, 50);
	_background_button = new Button(460, 1030, 180, 50);
	_background_option_button = new Button(640, 1030, 180, 50);
	_prefab_button = new Button(820, 1030, 180, 50);
	_prefab_option_button = new Button(1000, 1030, 180, 50);
	_set_file_name_button = new Button(1500, 50, 180, 50);
	_play_button = new Button(1500, 500, 180, 50);
	_file_name = new TextItem(1500, 130, 180, 50);
	_background_list = new ScrollList(460, 630, 180, 400);
	_background_list->setItemHeight(50.0f);
	_background_list->setFontSize(_font_size);
	_map_list = new ScrollList(280, 630, 180, 400);
	_map_list->setItemHeight(50.0f);
	_map_list->setFontSize(_font_size);
	_prefab_list = new ScrollList(820, 630, 180, 400);
	_prefab_list->setHeight(50.0f);
	_prefab_list->setFontSize(_font_size);

	_save_button->setButtonText(TString("save"), _font_size);
	_load_button->setButtonText(TString("load"), _font_size);
	_wall_button->setButtonText(TString("wall"), _font_size);
	_background_button->setButtonText(TString("background"), _font_size);
	_background_option_button->setButtonText(TString("b: "), _font_size);
	_prefab_button->setButtonText(TString("prefab"), _font_size);
	_prefab_option_button->setButtonText(TString("p: "), _font_size);
	_set_file_name_button->setButtonText(TString("file name"), _font_size);
	_play_button->setButtonText(TString("play"), _font_size);
	_file_name->setText(TString(""));
	_file_name->setFontSize(_font_size);

	_save_button->setCallback([=]() {
		string map_name = "./";
		map_name.append(_map_name);
		map_name.append(".level");
		ResourceManager::saveMap(map_name);
	});

	_wall_button->setCallback([=]() {
		state = SET_WALL;
		cout << "set wall" << endl;
	});

	_background_button->setCallback([=]() {
		state = SET_BACKGROUND;
		_background_list->setVisable(true);
		cout << "set background" << endl;
	});

	_prefab_button->setCallback([=]() {
		state = SET_PREFAB;
		_prefab_list->setVisable(true);
	});

	_load_button->setCallback([=]() {
		_map_list->setVisable(true);
	});

	_set_file_name_button->setCallback([=]() {
		state = SET_FILE_NAME;
	});

	_play_button->setCallback([=]() {
		if (_play_button->button_text->text == "play" && _map_name != "") {
			//save map first
			string map_name = "./";
			map_name.append(_map_name);
			map_name.append(".level");
			ResourceManager::saveMap(map_name);
			camera.setAngle(45);

			game->setCurrentPage("default");
			_play_button->setButtonText(TString("stop"), _font_size);
			_save_button->setVisable(false);
			_load_button->setVisable(false);
			_wall_button->setVisable(false);
			_background_button->setVisable(false);
			_background_option_button->setVisable(false);
			_prefab_button->setVisable(false);
			_prefab_option_button->setVisable(false);
			_set_file_name_button->setVisable(false);
			_file_name->setVisable(false);
			_background_list->setVisable(false);
			_map_list->setVisable(false);
			_prefab_list->setVisable(false);
			_save_button->button_text->setVisable(false);
			_load_button->button_text->setVisable(false);
			_wall_button->button_text->setVisable(false);
			_background_button->button_text->setVisable(false);
			_background_option_button->button_text->setVisable(false);
			_prefab_button->button_text->setVisable(false);
			_prefab_option_button->button_text->setVisable(false);
			_set_file_name_button->button_text->setVisable(false);
			ResourceManager::loadMap("./" + _map_name + ".level");
		}
		else {
			camera.setAngle(0);
			game->setCurrentPage("editor");
			_play_button->setButtonText(TString("play"), _font_size);
			_save_button->setVisable(true);
			_load_button->setVisable(true);
			_wall_button->setVisable(true);
			_background_button->setVisable(true);
			_background_option_button->setVisable(true);
			_prefab_button->setVisable(true);
			_prefab_option_button->setVisable(true);
			_set_file_name_button->setVisable(true);
			_file_name->setVisable(true);
			_save_button->button_text->setVisable(true);
			_load_button->button_text->setVisable(true);
			_wall_button->button_text->setVisable(true);
			_background_button->button_text->setVisable(true);
			_background_option_button->button_text->setVisable(true);
			_prefab_button->button_text->setVisable(true);
			_prefab_option_button->button_text->setVisable(true);
			_set_file_name_button->button_text->setVisable(true);
			ResourceManager::loadMap("./" + _map_name + ".level", 0);
		}
	});

	_background_option_button->setCallback([=]() {
		if (state == SET_NOTHING) {
			state = SET_BACKGROUND_SIZE;
			_background_option_button->setButtonText(TString(L"b: set size"), _font_size);
			return;
		}
		if (state == SET_BACKGROUND_SIZE) {
			state = DELETE_BACKGROUND;
			_background_option_button->setButtonText(TString("b: delete"), _font_size);
			return;
		}
		if (state == DELETE_BACKGROUND) {
			state = SET_NOTHING;
			_background_option_button->setButtonText(TString("b:"), _font_size);
			return;
		}
	});

	_prefab_option_button->setCallback([=]() {
		if (state == SET_NOTHING) {
			state = SET_PREFAB_POSITION;
			_prefab_option_button->setButtonText(TString("p: set pos"), _font_size);
			return;
		}
		if (state == SET_PREFAB_POSITION) {
			state = DELETE_PREFAB;
			_prefab_option_button->setButtonText(TString("p: delete"), _font_size);
			return;
		}
		if (state == DELETE_PREFAB) {
			state = SET_NOTHING;
			_prefab_option_button->setButtonText(TString("p: "), _font_size);
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
	ObjectManager::addUI(_prefab_button);
	ObjectManager::addText(_prefab_button->button_text);
	ObjectManager::addUI(_prefab_option_button);
	ObjectManager::addText(_prefab_option_button->button_text);
	ObjectManager::addUI(_play_button);
	ObjectManager::addText(_play_button->button_text);
	//ObjectManager::addUI(_background_list);
	//ObjectManager::addUI(_prefab_list);
	ObjectManager::addUI(_set_file_name_button);
	ObjectManager::addText(_set_file_name_button->button_text);
	ObjectManager::addText(_file_name);
	
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

void LevelEditor::initPrefabAdder(PREFAB_FUNCTION func)
{
	_prefab_adder = func;
	_prefab_has_init = true;
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
	if (state == SET_FILE_NAME) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "SET FILE NAME", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	if (state == SET_BACKGROUND_SIZE) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "SET BACKGROUND SIZE", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	if (state == DELETE_BACKGROUND) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "DELETE BACKGROUND", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	if (state == SET_PREFAB) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "SET PREFAB", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	if (state == SET_PREFAB_POSITION) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "SET PREFAB POSITION", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	if (state == DELETE_PREFAB) {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x) + "DELETE PREFAB", 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
	else {
		Render::drawText(TString().number(pos.z) + "\t" + TString().number(pos.x), 120, 120, 0.7, glm::vec3(1.0, 1.0, 1.0));
	}
}

void LevelEditor::setFontSize(int size)
{
	_font_size = size;
}

void LevelEditor::setPrefabList()
{
	for (int i = 0; i < ResourceManager::getPrefabList().size(); ++i) {
		_prefab_list->addItem(ResourceManager::getPrefabList().at(i), [=]() {
			_current_prefab = ResourceManager::getPrefabList().at(i);
			_prefab_list->setVisable(false);
		});
	}
	_prefab_list->setVisable(false);
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

void LevelEditor::setMapList()
{
	cout << "number of maps  " << ResourceManager::getMapList().size() << endl;
	for (int i = 0; i < ResourceManager::getMapList().size();  ++i) {
		_map_list->addItem(ResourceManager::getMapList().at(i), [=]() {
			ResourceManager::loadMap(ResourceManager::getMapList().at(i).c_str(), 1);
			_map_name = ResourceManager::getMapList().at(i).substr(2, ResourceManager::getMapList().at(i).size() - 8);
			_file_name->text = _map_name;
			_map_list->setVisable(false);
		});

		_map_list->setVisable(false);
	}
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
