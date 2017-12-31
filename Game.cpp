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

void Game::render()
{
	if (state == ACTIVATE) {
		while (!EventManager::render_event.empty()) {	//執行event
			EventManager::render_event.front()->use();
			delete EventManager::render_event.front();
			EventManager::render_event.pop();
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
		/*for (int i = 0; i < ObjectManager::object_list.size(); ++i) {      //old physics
			Physics::gravity(ObjectManager::object_list.at(i), dt);
			DrawEvent<Entity *> *event = new DrawEvent<Entity *>();
			event->setTarget(ObjectManager::object_list.at(i));
			EventManager::render_event.push(event);			//若需要向上轉型，必須手動轉型
		}
		//if (camera._is_moving) {
		//	ObjectManager::object_list.at(7)->rigid.data.velocity = camera.Front * 10.0f;
		//}
		//else {
		//	ObjectManager::object_list.at(7)->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		//}
		if (Physics::collisionDetect(ObjectManager::object_list.at(5), ObjectManager::object_list.at(6)) == -1) {

		}
		Physics::collisionReact(ObjectManager::object_list.at(5), ObjectManager::object_list.at(6), dt);
		Physics::collisionReact(ObjectManager::object_list.at(5), ObjectManager::object_list.at(7), dt);
		Physics::collisionReact(ObjectManager::object_list.at(6), ObjectManager::object_list.at(7), dt);/*/
		for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
			Physics::displace(ObjectManager::object_list.at(i), dt);
		}
	}
}

void Game::input()
{
	if (state == ACTIVATE) {

	}
}

