#include "Game.h"
#define SCENE_WIDTH 1280
#define SCENE_HEIGHT 720

//		debug function
void aabbDebuger();
void showVersion();

extern Camera camera;

bool is_player = false;

Game::Game()
{
	state = LEVEL_EDITOR;
	level_editor = new LevelEditor();
	scene = new Scene();
	player = new Player(glm::vec3(0, 0.5, 0));

	scene->setVSync(false);


	Init();
	level_editor->setBackgroundList();
	level_editor->setMapList();
}


Game::~Game()
{
	delete level_editor;
	delete scene;
}

void Game::Init()
{
	srand(time(NULL));

	glEnable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glLineWidth(2.5);

	Render::addShader("text", "./text.vs", "./text.fs");
	Render::addShader("texture", "./texture.vs", "./texture.fs");
	Render::addShader("texture_ins", "./texture_instance.vs", "texture.fs");

	Render::shaders.search("texture").use();
	Render::shaders.search("texture").setInt("material.diffuse", 0);

	Render::shaders.search("texture_ins").use();
	Render::shaders.search("texture_ins").setInt("material.diffuse", 0);

	Render::addShader("color", "./color.vs", "./color.fs");
	Render::addShader("color_ins", "./color_instance.vs", "./color_instance.fs");

	ResourceManager::loadResource("./resource.rec");
	ResourceManager::loadMapList("./");

	//		vbo for instance		//
	float vertices[1] = { 0 };
	Render::addVBO(vertices, 1, "instance");
	Render::addVBO(vertices, 1, "debuger");
	int ppp[1] = { 0 };
	int sss[1] = { 3 };
	int shh[1] = { 0 };
	Render::addVAO(ppp, sss, shh, 1, "debuger", "debuger");
	//		vbo for instance		//
	
	setCurrentPage("editor");
	setPageMouseFunction("editor", [this](GLFWwindow* window, double xpos, double ypos) {
		EditorMouseCallback(window, xpos, ypos);
	});
	setPageMouseClickFunction("editor", [this](GLFWwindow *window, int button, int action, int mods) {
		EditorMouseButtonCallback(window, button, action, mods);
	});
	setPageKeyFunction("editor", [this](Scene &sc, float dt) {
		EditorKeyCallback(sc, dt);
	});
	
	setPageMouseFunction("defalut", [this](GLFWwindow* window, double xpos, double ypos) {
		DefaultMouseCallback(window, xpos, ypos);
	});
	setPageMouseClickFunction("default", [this](GLFWwindow *window, int button, int action, int mods) {
		DefaultMouseButtonCallback(window, button, action, mods);
	});
	setPageScrollFunction("default", [this](GLFWwindow *window, double xoffset, double yoffset) {
		return DefaultScrollCallback(window, xoffset, yoffset);
	});
	setPageKeyFunction("default", [this](Scene &sc, float dt) {
		DefaultKeyCallback(sc, dt);
	});

	textInit();
	textureInit();
	uiInit();

	
}

void foo(){
	cout << "fuck" << endl;
}

void Game::render()
{
	if (state == ACTIVATE || state == LEVEL_EDITOR) {
		glm::mat4 projection;
		projection = camera.getProjectMatrix();
		glm::mat4 view;
		view = camera.getViewMatrix();

		////////////////////////////////////////	texture shader draw		////////////////////////////////////////
		(Render::shaders.search("texture_ins")).use();
		Render::shaders.search("texture_ins").setMat4("view", view);
		Render::shaders.search("texture_ins").setMat4("projection", projection);
		//*************************************			實例化渲染			**************************************//
		//*********************************可依照 texture 做 sorting 做分段實例化*********************************//
		{
			if (!EventManager::texture_render_event.empty()) {
				EventManager::eventSort(EventManager::texture_render_event);
				glm::vec4 *instMat;
				glBindVertexArray(Render::VAOs.search("instance_texture"));		//等貼圖問題解決後嘗試移到迴圈外
//
				int count = 0;
				int size = 0;
				string texture = EventManager::texture_render_event.at(0)->_target_id->texture;
				
				instMat = new glm::vec4[EventManager::texture_render_event.size() * 5];		
				bool _time_to_stop = false;
				do {
					glBindTexture(GL_TEXTURE_2D, Render::textures.search(texture));
 					for (int i = count; i < EventManager::texture_render_event.size(); ++i) {
						if (i == EventManager::texture_render_event.size() - 1) {
							count = i;
							_time_to_stop = true;
						}
						if (EventManager::texture_render_event.at(i)->_target_id->texture != texture){
							count = i;
							texture = EventManager::texture_render_event.at(i)->_target_id->texture;
							_time_to_stop = false;
							break;
						}
						
						glm::mat4 model;
						model = glm::translate(model, glm::vec3(EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.x, EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.y, EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.z));
						model = model * EventManager::texture_render_event.at(i)->_target_id->_model_matrix;
						instMat[size * 5] = EventManager::texture_render_event.at(i)->_target_id->_texture_cood;
						instMat[size * 5 + 1] = model[0];
						instMat[size * 5 + 2] = model[1];
						instMat[size * 5 + 3] = model[2];
						instMat[size * 5 + 4] = model[3];
						++size;
					}
					Render::updateInstance(instMat, size);
					
					glDrawArraysInstanced(GL_TRIANGLES, 0, 6, size);
					size = 0;
				} while (count < EventManager::texture_render_event.size() - 1 || !_time_to_stop);
				delete[]instMat;
			}
		}
		//*************************************			實例化渲染			**************************************//
		while (!EventManager::texture_render_event.empty()) {	//執行event
			//EventManager::texture_render_event.back()->use();
			delete EventManager::texture_render_event.back();
			EventManager::texture_render_event.pop_back();
		}
		////////////////////////////////////////	texture shader draw		////////////////////////////////////////

		////////////////////////////////////////	color shader draw		////////////////////////////////////////
		(Render::shaders.search("color_ins")).use();
		Render::shaders.search("color_ins").setMat4("view", view);
		Render::shaders.search("color_ins").setMat4("projection", projection);
		//*************************************			實例化渲染			**************************************//
		//*********************************可依照 texture 做 sorting 做分段實例化*********************************//
		{
			glm::mat4 *instMat;
			instMat = new glm::mat4[EventManager::color_render_event.size()];
			for (int i = 0; i < EventManager::color_render_event.size(); ++i) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(EventManager::color_render_event.at(i)->_target_id->rigid.data.position.x, EventManager::color_render_event.at(i)->_target_id->rigid.data.position.y, EventManager::color_render_event.at(i)->_target_id->rigid.data.position.z));
				instMat[i] = model;
			}
			Render::updateInstance(instMat, EventManager::color_render_event.size());
			glBindVertexArray(Render::VAOs.search("sphere"));
			glDrawArraysInstanced(GL_LINE_STRIP, 0, 1944, EventManager::color_render_event.size());
			delete[]instMat;
		}
		//**************************************		實例化渲染			**************************************//
		while (!EventManager::color_render_event.empty()) {	//執行event
			//EventManager::color_render_event.back()->use();
			delete EventManager::color_render_event.back();
			EventManager::color_render_event.pop_back();
		}
		////////////////////////////////////////	color shader draw		////////////////////////////////////////

		////////////////////////////////////////			draw ui			////////////////////////////////////////
		(Render::shaders.search("color")).use();
		if (!EventManager::editor_ui_event.empty()) {
			Render::shaders.search("color").setMat4("view", view);
			Render::shaders.search("color").setMat4("projection", projection);
			while (!EventManager::editor_ui_event.empty()) {
				EventManager::editor_ui_event.back()->_target_id->draw();
				delete EventManager::editor_ui_event.back();
				EventManager::editor_ui_event.pop_back();
			}
		}

		view = glm::mat4();
		projection= glm::ortho(0.0f, static_cast<GLfloat>(1920), 0.0f, static_cast<GLfloat>(1080), 10.0f, -10.0f);
		if (!EventManager::draw_color_ui_event.empty()) {
			Render::shaders.search("color").setMat4("view", view);
			Render::shaders.search("color").setMat4("projection", projection);
			while (!EventManager::draw_color_ui_event.empty()) {
				EventManager::draw_color_ui_event.back()->use();
				delete EventManager::draw_color_ui_event.back();
				EventManager::draw_color_ui_event.pop_back();
			}
		}

		if (!EventManager::draw_texture_ui_event.empty()) {
			(Render::shaders.search("texture")).use();
			Render::shaders.search("texture").setMat4("view", view);
			Render::shaders.search("texture").setMat4("projection", projection);
			while (!EventManager::draw_texture_ui_event.empty()) {
				EventManager::draw_texture_ui_event.back()->use();
				delete EventManager::draw_texture_ui_event.back();
				EventManager::draw_texture_ui_event.pop_back();
			}
		}

		if (!EventManager::draw_text_event.empty()) {
			(Render::shaders.search("text")).use();
			Render::shaders.search("text").setMat4("projection", projection);
			glBindVertexArray(Render::VAOs.search("text"));
			while (!EventManager::draw_text_event.empty()) {
				EventManager::draw_text_event.back()->use();
				delete EventManager::draw_text_event.back();
				EventManager::draw_text_event.pop_back();
			}
		}
		////////////////////////////////////////			draw ui			////////////////////////////////////////
	}
	
}

void Game::update(float dt)
{
	if (state == ACTIVATE || state == LEVEL_EDITOR) {
		while (!EventManager::update_event.empty()) { //執行event
			EventManager::update_event.front()->use();
			delete EventManager::update_event.front();
			EventManager::update_event.pop();
		}

		for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
			//if distance to player too far, don't do the displace//<----------------------some day will finish
			Physics::displace(ObjectManager::object_list.at(i), dt);
		}
		ObjectManager::aabb_tree.update();

		for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
			if (ObjectManager::getUI().at(i)->visable()) {
				DrawEvent<UI *> *event = new DrawEvent<UI *>();
				event->setTarget(ObjectManager::getUI().at(i));
				if (ObjectManager::getUI().at(i)->_editor) {
					EventManager::editor_ui_event.push_back(event);
				}
				else if (ObjectManager::getUI().at(i)->_texture == "") {
					EventManager::draw_color_ui_event.push_back(event);
				}
				else {
					EventManager::draw_texture_ui_event.push_back(event);
				}
			}
		}

		for (int i = 0; i < ObjectManager::getText().size(); ++i) {
			if (ObjectManager::getText().at(i)->visable()) {
				DrawEvent<TextItem *> *event = new DrawEvent<TextItem *>();
				event->setTarget(ObjectManager::getText().at(i));
				EventManager::draw_text_event.push_back(event);
			}
		}
	}
}

void Game::input(Scene &sc, float dt)
{
	if (getPageKeyFunction().find(_current_page) != getPageKeyFunction().end()) {
		getPageKeyFunction()[_current_page](sc, dt);
	}
	else {
		getPageKeyFunction()["default"](sc, dt);
	}
}

void Game::mouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	if (getPageMouseFunction().find(_current_page) != getPageMouseFunction().end()) {
		getPageMouseFunction()[_current_page](window, xpos, ypos);
	}
	else {
		getPageMouseFunction()["default"](window, xpos, ypos);
	}
}

void Game::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (getPageMouseClickFunction().find(_current_page) != getPageMouseClickFunction().end()) {
		getPageMouseClickFunction()[_current_page](window, button, action, mods);
	}
	else {
		getPageMouseClickFunction()["default"](window, button, action, mods);
	}
}

bool Game::scrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	if (getPageScrollFunction().find(_current_page) != getPageScrollFunction().end()) {
		return getPageScrollFunction()[_current_page](window, xoffset, yoffset);
	}
	else {
		return getPageScrollFunction()["default"](window, xoffset, yoffset);
	}
}

void Game::EditorKeyCallback(Scene & sc, float dt)
{
	if (level_editor->state == SET_FILE_NAME) {
		textInput(sc, level_editor->_map_name);
		level_editor->_file_name->setText(level_editor->_map_name);
		if (glfwGetKey(sc.window, GLFW_KEY_ENTER) == GLFW_PRESS) {
			level_editor->state = SET_NOTHING;
		}
		return;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(sc.window, true);
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_RELEASE && glfwGetKey(sc.window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(sc.window, GLFW_KEY_D) == GLFW_RELEASE) {
		camera._v.x *= 0.5;
		camera._v.z *= 0.5;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(SPACE, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processKeyboard(SHIFT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		if (sc.isFullScreen()) {
			sc.setFullScreen(false);
		}
		else {
			sc.setFullScreen(true);
		}
	}
	if (glfwGetKey(sc.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_PRESS) {
		string map_name = "./";
		map_name.append(level_editor->_map_name);
		map_name.append(".level");
		ResourceManager::saveMap(map_name);
	}
	if (glfwGetKey(sc.window, GLFW_KEY_TAB) == GLFW_PRESS) {
		//if (is_player) {
		//	is_player = false;
		//}
		//else {
		//	is_player = true;
		//}
	}
}

void Game::EditorMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	//將螢幕座標轉為投影座標
	_mouse_x = xpos;
	_mouse_y = SCENE_HEIGHT - ypos;
	_mouse_x /= SCENE_WIDTH;
	_mouse_y /= SCENE_HEIGHT;
	_mouse_x *= 1920;
	_mouse_y *= 1080;
	//				hover event				//
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
			//	break;
		}
		float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
		float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
		float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
		float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
		if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasHover() && ObjectManager::getUI().at(i)->visable()) {
			ObjectManager::getUI().at(i)->hoverIn();
		}
		else if (ObjectManager::getUI().at(i)->hasHover()) {
			ObjectManager::getUI().at(i)->hoverOut();
		}
	}
	//				hover event				//
	if (level_editor->_is_setting && level_editor->state == SET_WALL) {
		level_editor->current_UI->setPoint2(level_editor->mouse2map(_mouse_x, _mouse_y));
	}
	if (level_editor->_is_setting && level_editor->state == SET_BACKGROUND_SIZE) {
		glm::vec3 map_pos = level_editor->mouse2map(_mouse_x, _mouse_y);
		if (abs(map_pos.x - level_editor->current_entity->rigid.data.position.x) < 1) {
			float dx = map_pos.x - level_editor->current_entity->rigid.data.position.x;
			level_editor->current_entity->rigid.data.position.x = map_pos.x;
			level_editor->current_entity->setHeightWidth(level_editor->current_entity->_height - dx, level_editor->current_entity->_width);
		}
		if (abs(map_pos.z - level_editor->current_entity->rigid.data.position.z) < 1) {
			float dz = map_pos.z - level_editor->current_entity->rigid.data.position.z;
			level_editor->current_entity->rigid.data.position.z = map_pos.z;
			level_editor->current_entity->setHeightWidth(level_editor->current_entity->_height, level_editor->current_entity->_width - dz);
		}
		if (abs(map_pos.x - level_editor->current_entity->rigid.data.position.x - level_editor->current_entity->_height) < 1) {
			float dx = map_pos.x - level_editor->current_entity->rigid.data.position.x - level_editor->current_entity->_height;
			level_editor->current_entity->setHeightWidth(level_editor->current_entity->_height + dx, level_editor->current_entity->_width);
		}
		if (abs(map_pos.z - level_editor->current_entity->rigid.data.position.z - level_editor->current_entity->_width) < 1) {
			float dz = map_pos.z - level_editor->current_entity->rigid.data.position.z - level_editor->current_entity->_width;
			level_editor->current_entity->setHeightWidth(level_editor->current_entity->_height, level_editor->current_entity->_width + dz);
		}
	}
}

void Game::EditorMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		bool has_select = false;
		bool is_button = false;
		//		ui callback
		for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
			if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
				break;
			}
			float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
			float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
			float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
			float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
			if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasCallback() && ObjectManager::getUI().at(i)->visable()) {
				ObjectManager::getUI().at(i)->callback();
				is_button = true;
			}
			else if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && !ObjectManager::getUI().at(i)->hasCallback() && ObjectManager::getUI().at(i)->visable()) {
				level_editor->current_UI = ObjectManager::getUI().at(i);
				has_select = true;
			}
		}
		//		ui callback
		if (!has_select) {
			level_editor->current_UI = nullptr;
		}
		if (level_editor->state == SET_WALL && !is_button) {
			if (!level_editor->_is_setting) {
				WallUI *ui = new WallUI(level_editor->mouse2map(_mouse_x, _mouse_y));
				level_editor->_is_setting = true;
				level_editor->current_UI = ui;
				ObjectManager::addUI(ui);
			}
		}
		if (level_editor->state == SET_BACKGROUND && !is_button) {
			if (!level_editor->_is_setting) {
				BackgroundObject *background = new BackgroundObject(level_editor->_current_texture, level_editor->mouse2map(_mouse_x, _mouse_y) - glm::vec3(5, 0, 5), level_editor->mouse2map(_mouse_x, _mouse_y) + glm::vec3(5, 0, 5));
				level_editor->state = SET_NOTHING;
				ObjectManager::addEntity(background);
			}
		}
		if (level_editor->state == SET_BACKGROUND_SIZE && !is_button) {
			glm::vec3 map_pos = level_editor->mouse2map(_mouse_x, _mouse_y);
			for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
				if (ObjectManager::object_list.at(i)->e_type == BACKGROUND_ENTITY) {
					if (map_pos.x > ObjectManager::object_list.at(i)->rigid.data.position.x&&map_pos.z > ObjectManager::object_list.at(i)->rigid.data.position.z&&map_pos.x < ObjectManager::object_list.at(i)->rigid.data.position.x + ObjectManager::object_list.at(i)->_height&&map_pos.z < ObjectManager::object_list.at(i)->rigid.data.position.z + ObjectManager::object_list.at(i)->_width) {
						level_editor->current_entity = ObjectManager::object_list.at(i);
						level_editor->_is_setting = true;
					}
				}
			}
		}
		if (level_editor->state == DELETE_BACKGROUND && !is_button) {
			glm::vec3 map_pos = level_editor->mouse2map(_mouse_x, _mouse_y);
			for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
				if (ObjectManager::object_list.at(i)->e_type == BACKGROUND_ENTITY) {
					if (map_pos.x > ObjectManager::object_list.at(i)->rigid.data.position.x&&map_pos.z > ObjectManager::object_list.at(i)->rigid.data.position.z&&map_pos.x < ObjectManager::object_list.at(i)->rigid.data.position.x + ObjectManager::object_list.at(i)->_height&&map_pos.z < ObjectManager::object_list.at(i)->rigid.data.position.z + ObjectManager::object_list.at(i)->_width) {
						ObjectManager::removeEntity(ObjectManager::object_list.at(i));
					}
				}
			}
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		level_editor->_is_setting = false;
		level_editor->current_UI = nullptr;
		level_editor->current_entity = nullptr;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		level_editor->state = SET_NOTHING;
		level_editor->_background_list->setVisable(false);
		cout << "set nothing" << endl;

	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
			float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
			float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
			float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
			float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
			if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && !ObjectManager::getUI().at(i)->hasCallback() && ObjectManager::getUI().at(i)->visable()) {
				ObjectManager::removeUI(ObjectManager::getUI().at(i));
			}
		}
	}
}

void Game::DefaultKeyCallback(Scene & sc, float dt)
{
	if (glfwGetKey(sc.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(sc.window, true);
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_RELEASE && glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_RELEASE && glfwGetKey(sc.window, GLFW_KEY_A) == GLFW_RELEASE && glfwGetKey(sc.window, GLFW_KEY_D) == GLFW_RELEASE) {
		camera._v.x *= 0.5;
		camera._v.z *= 0.5;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(SPACE, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.processKeyboard(SHIFT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_BACKSPACE) == GLFW_PRESS) {
		if (sc.isFullScreen()) {
			sc.setFullScreen(false);
		}
		else {
			sc.setFullScreen(true);
		}
	}
	if (glfwGetKey(sc.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS && glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_PRESS) {
		string map_name = "./";
		map_name.append(level_editor->_map_name);
		map_name.append(".level");
		ResourceManager::saveMap(map_name);
	}
	if (glfwGetKey(sc.window, GLFW_KEY_TAB) == GLFW_PRESS) {
		//if (is_player) {
		//	is_player = false;
		//}
		//else {
		//	is_player = true;
		//}
	}
}

void Game::DefaultMouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	//將螢幕座標轉為投影座標
	_mouse_x = xpos;
	_mouse_y = SCENE_HEIGHT - ypos;
	_mouse_x /= SCENE_WIDTH;
	_mouse_y /= SCENE_HEIGHT;
	_mouse_x *= 1920;
	_mouse_y *= 1080;

	//				hover event				//
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
			//	break;
		}
		float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
		float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
		float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
		float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
		if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasHover() && ObjectManager::getUI().at(i)->visable()) {
			ObjectManager::getUI().at(i)->hoverIn();
		}
		else if (ObjectManager::getUI().at(i)->hasHover()) {
			ObjectManager::getUI().at(i)->hoverOut();
		}
	}
	//				hover event				//
}

void Game::DefaultMouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//		ui callback
		for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
			if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
				//	break;
			}
			float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
			float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
			float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
			float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
			if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasCallback() && ObjectManager::getUI().at(i)->visable()) {
				ObjectManager::getUI().at(i)->callback();
				cout << "call" << endl;
			}
		}
		//		ui callback
	}
}

bool Game::DefaultScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
	//		檢查是否有 UI 有滾輪事件，如果有則回傳 true
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
		float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
		float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
		float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
		if (ObjectManager::getUI().at(i)->hasWheelEvent() && left < _mouse_x && right > _mouse_x && top > _mouse_y && down < _mouse_y && ObjectManager::getUI().at(i)->enable()) {
			ObjectManager::getUI().at(i)->wheelEvent(yoffset);
			return true;
		}
	}
	return false;
}


void Game::textInput(Scene &sc, TString & text)
{
	int current_press = clock();
	if (glfwGetKey(sc.window, GLFW_KEY_1) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("1"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_2) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("2"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_3) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("3"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_4) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("4"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_5) == GLFW_PRESS && current_press - _last_press > 150) {

		text.append(string("5"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_6) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("6"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_7) == GLFW_PRESS && current_press - _last_press > 150) {

		text.append(string("7"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_8) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("8"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_9) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("9"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_0) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append(string("0"));

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_A) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("A"));
		}
		else {
			text.append(TString("a"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_B) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("B"));
		}
		else {
			text.append(TString("b"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_C) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("C"));
		}
		else {
			text.append(TString("c"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_D) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("D"));
		}
		else {
			text.append(TString("d"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_E) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("E"));
		}
		else {
			text.append(TString("e"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_F) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("F"));
		}
		else {
			text.append(TString("f"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_G) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("G"));
		}
		else {
			text.append(TString("g"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_H) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("H"));
		}
		else {
			text.append(TString("h"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_I) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("I"));
		}
		else {
			text.append(TString("i"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_J) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("J"));
		}
		else {
			text.append(TString("j"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_K) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("K"));
		}
		else {
			text.append(TString("k"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_L) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("L"));
		}
		else {
			text.append(TString("l"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_M) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("M"));
		}
		else {
			text.append(TString("m"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_N) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("N"));
		}
		else {
			text.append(TString("n"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_O) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("O"));
		}
		else {
			text.append(TString("o"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_P) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("P"));
		}
		else {
			text.append(TString("p"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_Q) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("Q"));
		}
		else {
			text.append(TString("q"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_R) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("R"));
		}
		else {
			text.append(TString("r"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("S"));
		}
		else {
			text.append(TString("s"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_T) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("T"));
		}
		else {
			text.append(TString("t"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_U) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("U"));
		}
		else {
			text.append(TString("u"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_V) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("V"));
		}
		else {
			text.append(TString("v"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("W"));
		}
		else {
			text.append(TString("w"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_X) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("X"));
		}
		else {
			text.append(TString("x"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_Y) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("Y"));
		}
		else {
			text.append(TString("y"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_Z) == GLFW_PRESS && current_press - _last_press > 100) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append(TString("Z"));
		}
		else {
			text.append(TString("z"));
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && current_press - _last_press > 50) {
		if (!text.data().empty()) {
			text.data().pop_back();
		}
		_last_press = current_press;
	}
}

void Game::textInput(Scene & sc, string & text)
{
	int current_press = clock();
	if (glfwGetKey(sc.window, GLFW_KEY_1) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("1");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_2) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("2");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_3) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("3");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_4) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("4");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_5) == GLFW_PRESS && current_press - _last_press > 150) {

		text.append("5");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_6) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("6");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_7) == GLFW_PRESS && current_press - _last_press > 150) {

		text.append("7");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_8) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("8");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_9) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("9");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_0) == GLFW_PRESS && current_press - _last_press > 150) {
		text.append("0");

		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_A) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("A");
		}
		else {
			text.append("a");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_B) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("B");
		}
		else {
			text.append("b");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_C) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("C");
		}
		else {
			text.append("c");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_D) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("D");
		}
		else {
			text.append("d");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_E) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("E");
		}
		else {
			text.append("e");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_F) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("F");
		}
		else {
			text.append("f");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_G) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("G");
		}
		else {
			text.append("g");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_H) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("H");
		}
		else {
			text.append("h");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_I) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("I");
		}
		else {
			text.append("i");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_J) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("J");
		}
		else {
			text.append("j");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_K) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("K");
		}
		else {
			text.append("k");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_L) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("L");
		}
		else {
			text.append("l");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_M) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("M");
		}
		else {
			text.append("m");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_N) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("N");
		}
		else {
			text.append("n");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_O) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("O");
		}
		else {
			text.append("o");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_P) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("P");
		}
		else {
			text.append("p");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_Q) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("Q");
		}
		else {
			text.append("q");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_R) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("R");
		}
		else {
			text.append("r");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("S");
		}
		else {
			text.append("s");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_T) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("T");
		}
		else {
			text.append("t");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_U) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("U");
		}
		else {
			text.append("u");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_V) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("V");
		}
		else {
			text.append("v");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("W");
		}
		else {
			text.append("w");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_X) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("X");
		}
		else {
			text.append("x");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_Y) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("Y");
		}
		else {
			text.append("y");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_Z) == GLFW_PRESS && current_press - _last_press > 150) {
		if (glfwGetKey(sc.window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS || glfwGetKey(sc.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			text.append("Z");
		}
		else {
			text.append("z");
		}
		_last_press = current_press;
	}
	if (glfwGetKey(sc.window, GLFW_KEY_BACKSPACE) == GLFW_PRESS && current_press - _last_press > 100) {
		if (!text.empty()) {
			text.pop_back();
		}
		_last_press = current_press;
	}
}



void Game::setCurrentPage(string key)
{
	_current_page = key;
}



void Game::textInit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	unsigned int VAO, VBO;
	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 5, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(4 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBindVertexArray(0);
	Render::addVAO("text", VAO);
	Render::addVBO("text", VBO);
}

void Game::textureInit()
{
	float vbo_vertex[] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	};
	Render::addVBO(vbo_vertex, 30, "instance_texture");
	int pos[2] = { 0, 1 };
	int size[2] = { 3, 2 };
	int shift[2] = { 0, 3 };
	Render::addVAO(pos, size, shift, 2, "instance_texture", "instance_texture");

}

void Game::uiInit()
{
	float vbo_vertex[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};
	float line_vbo[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};
	Render::addVBO(vbo_vertex, 18, "ui_vbo");
	Render::addVBO(line_vbo, 6, "line_vbo");
	int pos[1] = { 0 };
	int size[1] = { 3 };
	int shift[1] = { 0 };
	Render::addVAO(pos, size, shift, 1, "ui_vao", "ui_vbo");
	Render::addVAO(pos, size, shift, 1, "line_vao", "line_vbo");
}

std::map<string, MOUSE_CALL_BACK>& Game::getPageMouseFunction()
{
	return _page_mouse_function;
}

std::map<string, MOUSE_BUTTON_CALL_BACK>& Game::getPageMouseClickFunction()
{
	return _page_mouse_click_function;
}

std::map<string, KEY_CALL_BACK>& Game::getPageKeyFunction()
{
	return _page_key_function;
}

std::map<string, SCROLL_CALL_BACK>& Game::getPageScrollFunction()
{
	return _page_scroll_function;
}

void Game::setPageMouseFunction(string key, MOUSE_CALL_BACK func)
{
	_page_mouse_function[key] = func;
}

void Game::setPageMouseClickFunction(string key, MOUSE_BUTTON_CALL_BACK func)
{
	_page_mouse_click_function[key] = func;
}

void Game::setPageKeyFunction(string key, KEY_CALL_BACK func)
{
	_page_key_function[key] = func;
}

void Game::setPageScrollFunction(string key, SCROLL_CALL_BACK func)
{
	_page_scroll_function[key] = func;
}

void Game::gameLoop()
{
	int lastClock = clock();
	int count = 0;
	//////////////////////////////////////	start of the main loop	//////////////////////////////////////
	while (!glfwWindowShouldClose(scene->window)) {
		float current_frame = glfwGetTime();
		dt = current_frame - last_frame;
		last_frame = current_frame;
		/////////////////////////////////	display fps
		int currentClock = clock();
		if (currentClock - lastClock < 1000) {
			++count;
		}
		else {
			lastClock = currentClock;
			std::cerr << "fps : " << count << endl;
			count = 0;
		}
		/////////////////////////////////	display fps

		glClearColor(0.3, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// start to rending

		input(*scene, dt);

		if (is_player) {
			player->rigid.data.velocity = camera._v;
		}

		update(dt);

		if (is_player) {
			camera.Position = player->rigid.data.position - camera.radius * camera.Front;
			camera._v = player->rigid.data.velocity;
		}
		else {
			camera.Position += camera._v * dt;
		}
		render();


		//aabbDebuger();
		showVersion();
		level_editor->drawBaseLine();

		// end of loop
		while (1) {//控制FPS
			if (clock() - currentClock > 0) {
				break;
			}
		}
		//////////////////////////////////////	end of the main loop	//////////////////////////////////////
		glfwSwapBuffers(scene->window);
		glfwPollEvents();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}













//					debug function				//
void aabbDebuger() {
	//////////////////////////////////////////////////////////////////////draw aabb

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
	glm::mat4 view;
	view = camera.getViewMatrix();
	glm::mat4 model;
	(Render::shaders.search("color")).use();
	Render::shaders.search("color").setMat4("view", view);
	Render::shaders.search("color").setMat4("projection", projection);
	Render::shaders.search("color").setMat4("model", model);

	AABBNode *current = ObjectManager::aabb_tree._root;
	std::queue<AABBNode *> que;
	que.push(current);
	while (!que.empty()) {
		Render::drawAABB(current->_aabb);
		if (!current->isLeaf()) {
			que.push(current->_children[0]);
			que.push(current->_children[1]);
		}
		current = que.front();
		que.pop();
	}

	//////////////////////////////////////////////////////////////////////draw aabb
}

void showVersion() {
	glm::vec3 color(0.9, 0.9f, 0.9f);
	glm::vec2 pos(900.0f, 1030.0f);
	TString tex(SOFTWARE_VERSION);
	Render::drawText(tex, pos.x, pos.y, 0.7, color);
}


