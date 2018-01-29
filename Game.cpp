#include "Game.h"

extern TPCamera TPcamera;

Game::Game()
{
	state = ACTIVATE;
}


Game::~Game()
{
}

void Game::Init()
{
}

void foo(){
	cout << "fuck" << endl;
}

void Game::render()
{
	if (state == ACTIVATE) {
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
		glm::mat4 view;
		view = TPcamera.getViewMatrix();


		(Render::shaders.search("texture")).use();
		Render::shaders.search("texture").setMat4("view", view);
		Render::shaders.search("texture").setMat4("projection", projection);
		while (!EventManager::texture_render_event.empty()) {	//°õ¦æevent
			EventManager::texture_render_event.back()->use();
			delete EventManager::texture_render_event.back();
			EventManager::texture_render_event.pop_back();
		}

		(Render::shaders.search("color")).use();
		Render::shaders.search("color").setMat4("view", view);
		Render::shaders.search("color").setMat4("projection", projection);
//		glm::mat4 *instMat;
//		instMat = new glm::mat4[EventManager::color_render_event.size()];
//		for (int i = 0; i < EventManager::color_render_event.size(); ++i) {
//			glm::mat4 model;
//			model = glm::translate(model, glm::vec3(EventManager::color_render_event.at(i)->_target_id->rigid.data.position.x, EventManager::color_render_event.at(i)->_target_id->rigid.data.position.y, EventManager::color_render_event.at(i)->_target_id->rigid.data.position.z));
//			instMat[i] = model;
//		}
//		Render::updateInstance(instMat, EventManager::color_render_event.size());
//		glBindVertexArray(Render::VAOs.search("sphere"));
//		glDrawArraysInstanced(GL_LINE_STRIP, 0, 1944, EventManager::color_render_event.size());
//		delete []instMat;
		while (!EventManager::color_render_event.empty()) {	//°õ¦æevent
			EventManager::color_render_event.back()->use();
			delete EventManager::color_render_event.back();
			EventManager::color_render_event.pop_back();
		}

	}
	
}

void Game::update(float dt)
{
	if (state == ACTIVATE) {
		while (!EventManager::update_event.empty()) { //°õ¦æevent
			EventManager::update_event.front()->use();
			delete EventManager::update_event.front();
			EventManager::update_event.pop();
		}

		for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
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

