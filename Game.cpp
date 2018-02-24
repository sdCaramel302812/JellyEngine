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

	textInit();
	textureInit();
	uiInit();

	//		vbo for instance		//
	float vertices[1] = { 0 };
	Render::addVBO(vertices, 1, "instance");
	Render::addVBO(vertices, 1, "debuger");
	int ppp[1] = { 0 };
	int sss[1] = { 3 };
	int shh[1] = { 0 };
	Render::addVAO(ppp, sss, shh, 1, "debuger", "debuger");
	//		vbo for instance		//
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
		(Render::shaders.search("texture")).use();
		Render::shaders.search("texture").setMat4("view", view);
		Render::shaders.search("texture").setMat4("projection", projection);
		//*************************************			實例化渲染			**************************************//
		//*********************************可依照 texture 做 sorting 做分段實例化*********************************//
/*		{
			if (!EventManager::texture_render_event.empty()) {
				EventManager::eventSort(EventManager::texture_render_event);
				glm::mat4 *instMat;

				int count = 0;
				int size = 0;
				string texture = EventManager::texture_render_event.at(0)->_target_id->texture;
				
				instMat = new glm::mat4[EventManager::texture_render_event.size()];		
				while (count < EventManager::texture_render_event.size() - 1) {
					glBindTexture(GL_TEXTURE_2D, Render::textures.search(texture));
					for (int i = count; i < EventManager::texture_render_event.size(); ++i) {
						if (i == EventManager::texture_render_event.size() - 1) {
							count = i;
						}
						if (EventManager::texture_render_event.at(i)->_target_id->texture != texture){
							count = i;
							texture = EventManager::texture_render_event.at(i)->_target_id->texture;
							break;
						}
						++size;
						glm::mat4 model;
						model = glm::translate(model, glm::vec3(EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.x, EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.y, EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.z));
						model = model * EventManager::texture_render_event.at(i)->_target_id->_model_matrix;
						instMat[i] = model;
					}
					Render::updateInstance(instMat, size);
					glBindVertexArray(Render::VAOs.search("instance_texture"));
					glDrawArraysInstanced(GL_TRIANGLES, 0, 6, size);
					size = 0;
				}
				delete[]instMat;
			}
		}
		//*************************************			實例化渲染			**************************************///
		while (!EventManager::texture_render_event.empty()) {	//執行event
			EventManager::texture_render_event.back()->use();
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
			//if distance to player to far, don't do the displace//<----------------------some day will finish
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
	if (state == ACTIVATE || state == LEVEL_EDITOR) {
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
		if (glfwGetKey(sc.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			if (sc.isFullScreen()) {
				sc.setFullScreen(false);
			}
			else {
				sc.setFullScreen(true);
			}
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
}

void Game::mouseCallback(GLFWwindow * window, double xpos, double ypos)
{
	_mouse_x = xpos;
	_mouse_y = SCENE_HEIGHT - ypos;
	_mouse_x /= SCENE_WIDTH;
	_mouse_y /= SCENE_HEIGHT;
	_mouse_x *= 1920;
	_mouse_y *= 1080;
	//cout << _mouse_x << "\t" << _mouse_y << endl;
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
		//	break;
		}
		float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
		float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
		float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
		float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
		if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasHover()) {
			ObjectManager::getUI().at(i)->hoverIn();
		}
		else if(ObjectManager::getUI().at(i)->hasHover()){
			ObjectManager::getUI().at(i)->hoverOut();
		}
	}
	if (state == LEVEL_EDITOR) {
		if (level_editor->_is_setting && level_editor->state == SET_WALL) {
			level_editor->current_UI->setPoint2(level_editor->mouse2map(_mouse_x, _mouse_y));
		}
	}
}

void Game::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (state == ACTIVATE) {
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
				if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasCallback()) {
					ObjectManager::getUI().at(i)->callback();
					cout << "call" << endl;
				}
			}
			//		ui callback
		}
	}
	if (state == LEVEL_EDITOR) {
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
				if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasCallback()) {
					ObjectManager::getUI().at(i)->callback();
					is_button = true;
				}
				else if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && !ObjectManager::getUI().at(i)->hasCallback()) {
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
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
			level_editor->_is_setting = false;
			level_editor->current_UI = nullptr;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
			level_editor->state = SET_NOTHING;
			cout << "set nothing" << endl;
			
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
			for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
				float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
				float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
				float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
				float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
				if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && !ObjectManager::getUI().at(i)->hasCallback()) {
					ObjectManager::removeUI(ObjectManager::getUI().at(i));
				}
			}
		}
	}

}

void Game::mouseButtonCallback(bool mouse_state)
{
	if (mouse_state) {
		for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
			if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
				break;
			}
			float left = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() : ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width();
			float right = (ObjectManager::getUI().at(i)->width() > 0) ? ObjectManager::getUI().at(i)->x() + ObjectManager::getUI().at(i)->width() : ObjectManager::getUI().at(i)->x();
			float top = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height() : ObjectManager::getUI().at(i)->y();
			float down = (ObjectManager::getUI().at(i)->height() > 0) ? ObjectManager::getUI().at(i)->y() : ObjectManager::getUI().at(i)->y() + ObjectManager::getUI().at(i)->height();
			if (_mouse_x < right + 10 && _mouse_x > left - 10 && _mouse_y < top + 10 && _mouse_y > down - 10 && ObjectManager::getUI().at(i)->hasCallback()) {
				ObjectManager::getUI().at(i)->callback();
			}
		}
	}
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
