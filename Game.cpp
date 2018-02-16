#include "Game.h"

extern Camera camera;

Game::Game()
{
	state = ACTIVATE;
}


Game::~Game()
{
}

void Game::Init()
{
	srand(time(NULL));

	glEnable(GL_DEPTH_TEST);

	Render::addShader("text", "./text.vs", "./text.fs");
	Render::addShader("texture", "./texture.vs", "./texture.fs");
	Render::addShader("texture_ins", "./texture_instance.vs", "texture.fs");
	Render::addShader("color", "./color.vs", "./color.fs");
	Render::addShader("color_ins", "./color_instance.vs", "./color.fs");
	textInit();
}

void foo(){
	cout << "fuck" << endl;
}

void Game::render()
{
	if (state == ACTIVATE) {
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
			glm::mat4 *instMat;
			glActiveTexture(GL_TEXTURE0);
			string texture = EventManager::texture_render_event.at(0)->_target_id->texture;
			glBindTexture(GL_TEXTURE_2D, Render::textures.search(texture));
			instMat = new glm::mat4[EventManager::texture_render_event.size()];		
			for (int i = 0; i < EventManager::texture_render_event.size(); ++i) {
				glm::mat4 model;
				model = glm::translate(model, glm::vec3(EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.x, EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.y, EventManager::texture_render_event.at(i)->_target_id->rigid.data.position.z));
				model = model * EventManager::texture_render_event.at(i)->_target_id->_model_matrix;
				instMat[i] = model;
			}
			Render::updateInstance(instMat, EventManager::texture_render_event.size());
			glBindVertexArray(Render::VAOs.search("instance_texture"));
			glDrawArraysInstanced(GL_TRIANGLES, 0, 6, EventManager::texture_render_event.size());
			delete[]instMat;
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
		for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
			if(ObjectManager::getUI().at(i)->visable()){
				ObjectManager::getUI().at(i)->draw();

			}
		}
		////////////////////////////////////////			draw ui			////////////////////////////////////////
	}
	
}

void Game::update(float dt)
{
	if (state == ACTIVATE) {
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
	}
}

void Game::input(Scene &sc, float dt)
{
	if (state == ACTIVATE) {
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
	_mouse_y = ypos;
	//cout << xpos << "\t" << ypos << endl;
	for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
		if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
			break;
		}
		if (_mouse_x - ObjectManager::getUI().at(i)->x() < ObjectManager::getUI().at(i)->width() && _mouse_x > ObjectManager::getUI().at(i)->x() && _mouse_y - ObjectManager::getUI().at(i)->y() < ObjectManager::getUI().at(i)->height() && _mouse_y > ObjectManager::getUI().at(i)->y() && ObjectManager::getUI().at(i)->hasHover()) {
			ObjectManager::getUI().at(i)->hover();
		}
	}
}

void Game::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods)
{
	if (state == ACTIVATE) {
		if (glfwGetKey(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
			for (int i = 0; i < ObjectManager::getUI().size(); ++i) {
				if (ObjectManager::getUI().at(i)->x() > _mouse_x) {
					break;
				}
				if (_mouse_x - ObjectManager::getUI().at(i)->x() < ObjectManager::getUI().at(i)->width() && _mouse_x > ObjectManager::getUI().at(i)->x() && _mouse_y - ObjectManager::getUI().at(i)->y() < ObjectManager::getUI().at(i)->height() && _mouse_y > ObjectManager::getUI().at(i)->y() && ObjectManager::getUI().at(i)->hasCallback()) {
					ObjectManager::getUI().at(i)->callback();
					cout << "call" << endl;
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
			if (_mouse_x - ObjectManager::getUI().at(i)->x() < ObjectManager::getUI().at(i)->width() && _mouse_x > ObjectManager::getUI().at(i)->x() && _mouse_y - ObjectManager::getUI().at(i)->y() < ObjectManager::getUI().at(i)->height() && _mouse_y > ObjectManager::getUI().at(i)->y() && ObjectManager::getUI().at(i)->hasCallback()) {
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
		-0.5f,  0.0f,  -0.5f,
		0.5f, 0.0f, -0.5f,
		-0.5f,  0.0f, 0.5f,
		-0.5f, 0.0f, 0.5f,
		0.5f,  0.0f,  -0.5f,
		0.5f, 0.0f,  0.5f,
	};
	Render::addVBO(vbo_vertex, 18, "instance_texture");
	int pos[1] = { 0 };
	int size[1] = { 3 };
	int shift[1] = { 0 };
	Render::addVAO(pos, size, shift, 1, "instance_texture", "instance_texture");
	Render::addShader("instance_texture", "./texture_instance.vs", "./texture.fs");
	Render::addShader("texture", "./texture.vs", "./texture.fs");
}

void Game::uiInit()
{
	float vbo_vertex[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f,
	};
	Render::addVBO(vbo_vertex, 18, "ui_vbo");
	int pos[1] = { 0 };
	int size[1] = { 3 };
	int shift[1] = { 0 };
	Render::addVAO(pos, size, shift, 1, "ui_vao", "ui_vbo");
}
