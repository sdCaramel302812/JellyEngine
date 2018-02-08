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


		(Render::shaders.search("texture")).use();
		Render::shaders.search("texture").setMat4("view", view);
		Render::shaders.search("texture").setMat4("projection", projection);
		while (!EventManager::texture_render_event.empty()) {	//執行event
			EventManager::texture_render_event.back()->use();
			delete EventManager::texture_render_event.back();
			EventManager::texture_render_event.pop_back();
		}

		(Render::shaders.search("color_ins")).use();
		Render::shaders.search("color_ins").setMat4("view", view);
		Render::shaders.search("color_ins").setMat4("projection", projection);
		//*****************************************************		實例化渲染		*****************************************************//
		//*******************************************可依照 texture 做 sorting 做分段實例化**********************************************//
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
		delete []instMat;
		//*****************************************************		實例化渲染		*****************************************************//
		while (!EventManager::color_render_event.empty()) {	//執行event
			//EventManager::color_render_event.back()->use();
			delete EventManager::color_render_event.back();
			EventManager::color_render_event.pop_back();
		}

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

void Game::input()
{
	if (state == ACTIVATE) {

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
