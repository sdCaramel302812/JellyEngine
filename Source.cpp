#define SOFTWARE_VERSION "ver 0.4.3"

// GLEW ( help you using functions without retreiving functions )
#define _WIN32_WINNT 0x0500

#define SCENE_WIDTH 1280
#define SCENE_HEIGHT 720

#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>
#include <Windows.h>
#include <Winuser.h>
#include "Scene.h"
#include "Shader.h"
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <cmath>
#include "Camera.h"
#include <vector>
#include "Entity.h"
#include "Render.h"
#include "Game.h"
#include <stdlib.h>
#include "Button.h"
#include "LevelEditor.h"
//#include "Model.h"

// Get error from GLFW for debuging
//void error_callback(int error, const char* description);
// Get keywords from GLFW windows
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


Camera camera(2);
float dt = 0;
float last_frame = 0;

bool is_player = false;

void processInput(Scene &sc)
{
	if (glfwGetKey(sc.window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {

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
	if (glfwGetKey(sc.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		if (sc.isFullScreen()) {
			sc.setFullScreen(false);
		}
		else {
			sc.setFullScreen(true);
		}
	}
	if (glfwGetKey(sc.window, GLFW_KEY_TAB) == GLFW_PRESS) {
		if (is_player) {
			is_player = false;
		}
		else {
			is_player = true;
		}
	}
}

bool firstMouse = true;
float lastX = 1080 / 2, lastY = 760 / 2;


double mouse_x;
double mouse_y;
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_x = xpos;
	mouse_y = ypos;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

								  //	std::cerr << "xoffset : " << xoffset << std::endl;

	lastX = xpos;
	lastY = ypos;
	camera.processMouseEvent(xoffset, yoffset);
}

bool is_mouse_press = false;
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		is_mouse_press = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		is_mouse_press = false;
	}
}

//typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
//PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;




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

int main() {
	Scene sc;

	srand(time(NULL));
	sc.setVSync(false);
	//Shader shader("./vertex.vs", "./fragment.fs");
	//Shader colorShader("./color.vs", "./color.fs");
	//Shader groundShader("./vertex.vs", "./ground.fs");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	glfwSetCursorPosCallback(sc.window, mouse_callback);
	glfwSetMouseButtonCallback(sc.window, mouse_button_callback);

	float vertices[1] = { 0 };
	Render::addVBO(vertices, 1, "instance");
	Render::addVBO(vertices, 1, "debuger");
	int ppp[1] = { 0 };
	int sss[1] = { 3 };
	int shh[1] = { 0 };
	Render::addVAO(ppp, sss, shh, 1, "debuger","debuger");
	//封裝測試
	Game game;
	game.Init();
	game.textureInit();
	game.uiInit();
	
	//LevelEditor editor;

	Button *button = new Button(200, 200, 300, 100);
	button->setButtonText("button");
	button->setCallback([=]() {
		std::cout << "click" << endl;
	});
	//ObjectManager::addUI(button);


	int pos[3] = { 0,1,2 };
	int size[3] = { 3,2,3 };
	int shift[3] = { 0,5,3 };

	//sphere
	{
		float sphereLine[5832];
		float PI = 3.14159;
		int countLine = 0;
		int r_inter = 20;
		for (float Theta = 0; Theta < 360; Theta += r_inter) {
			for (float Phi = 0; Phi < 360; Phi += r_inter) {
				float x = 0.5 * cos(glm::radians(Phi)) * sin(glm::radians(Theta));
				float y = 0.5 * cos(glm::radians(Theta));
				float z = 0.5 * sin(glm::radians(Phi)) * sin(glm::radians(Theta));
				sphereLine[countLine] = x;
				sphereLine[countLine + 1] = y;
				sphereLine[countLine + 2] = z;
				countLine += 3;
				x = 0.5 * cos(glm::radians(Phi + r_inter)) * sin(glm::radians(Theta));
				y = 0.5 * cos(glm::radians(Theta));
				z = 0.5 * sin(glm::radians(Phi + r_inter)) * sin(glm::radians(Theta));
				sphereLine[countLine] = x;
				sphereLine[countLine + 1] = y;
				sphereLine[countLine + 2] = z;
				countLine += 3;
				x = 0.5 * cos(glm::radians(Phi)) * sin(glm::radians(Theta + r_inter));
				y = 0.5 * cos(glm::radians(Theta + r_inter));
				z = 0.5 * sin(glm::radians(Phi)) * sin(glm::radians(Theta + r_inter));
				sphereLine[countLine] = x;
				sphereLine[countLine + 1] = y;
				sphereLine[countLine + 2] = z;
				countLine += 3;
				x = 0.5 * cos(glm::radians(Phi + r_inter)) * sin(glm::radians(Theta));
				y = 0.5 * cos(glm::radians(Theta));
				z = 0.5 * sin(glm::radians(Phi + r_inter)) * sin(glm::radians(Theta));
				sphereLine[countLine] = x;
				sphereLine[countLine + 1] = y;
				sphereLine[countLine + 2] = z;
				countLine += 3;
				x = 0.5 * cos(glm::radians(Phi)) * sin(glm::radians(Theta + r_inter));
				y = 0.5 * cos(glm::radians(Theta + r_inter));
				z = 0.5 * sin(glm::radians(Phi)) * sin(glm::radians(Theta + r_inter));
				sphereLine[countLine] = x;
				sphereLine[countLine + 1] = y;
				sphereLine[countLine + 2] = z;
				countLine += 3;
				x = 0.5 * cos(glm::radians(Phi + r_inter)) * sin(glm::radians(Theta + r_inter));
				y = 0.5 * cos(glm::radians(Theta + r_inter));
				z = 0.5 * sin(glm::radians(Phi + r_inter)) * sin(glm::radians(Theta + r_inter));
				sphereLine[countLine] = x;
				sphereLine[countLine + 1] = y;
				sphereLine[countLine + 2] = z;
				countLine += 3;
			}
		}
		unsigned int ebo[6] = { 0,1,2,1,2,3 };
		Render::addVBO(sphereLine, 5832, "sphere");
		Render::addEBO(ebo, 6, "sphere");
		int pos[1] = { 0 };
		int size[1] = { 3 };
		int shift[1] = { 0 };
		Render::addVAO(pos, size, shift, 1, "sphere", "sphere", "sphere");
		//Render::addShader("color", "./color.vs", "./color.fs");
		//Render::addShader("color_ins", "./color_instance.vs", "./color_instance.fs");
		//*
		for (int i = 0; i < 2; ++i) {
			Entity *sphere = new Entity(1944);
			sphere->VAO = "sphere";
			sphere->VBO = "sphere";
			//sphere->EBO = "sphere";
			sphere->shader = "color_ins";
			sphere->rigid._is_static = false;
			sphere->rigid.data.position = glm::vec3(rand()%25, rand()%25, rand()%25);
			sphere->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
			sphere->rigid.setRadius(0.5);
			sphere->rigid._mass = 1;
			sphere->rigid._restitution_coeffient = 0.8;
			sphere->rigid.type = SPHERE;
			ObjectManager::object_list.push_back(sphere);
			ObjectManager::aabb_tree.add(sphere);
		}
		//*/
		Entity *sphere1 = new Entity(1944);
		sphere1->VAO = "sphere";
		sphere1->VBO = "sphere";
		//sphere->EBO = "sphere";
		sphere1->shader = "color_ins";
		sphere1->rigid._is_static = false;
		sphere1->rigid.data.position = glm::vec3(0.0f, 5.0f, 0.0f);
		sphere1->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		sphere1->rigid.setRadius(0.5);
		sphere1->rigid._mass = 100;
		sphere1->rigid._restitution_coeffient = 0.8;
		sphere1->rigid.type = SPHERE;
		ObjectManager::object_list.push_back(sphere1);
		ObjectManager::aabb_tree.add(sphere1);
	}
	//sphere

	Player *player = new Player(1944);
	player->rigid.setRadius(0.5);
	player->rigid._mass = 50;
	player->VAO = "sphere";
	player->VBO = "sphere";
	//sphere->EBO = "sphere";
	player->shader = "color_ins";
	player->rigid.data.position = glm::vec3(0.0f, 4.0f, 0.5f);
	//player->rigid.data.position = camera.Position + camera.radius * camera.Front;
	player->rigid._restitution_coeffient = 0;
	player->rigid.type = SPHERE;
	player->rigid._is_static = false;
	player->e_type = PLAYER;
	ObjectManager::object_list.push_back(player);
	ObjectManager::aabb_tree.add(player);
	
	//surface
	float surfaceVer[] = {
		5.0f,  5.0f,  5.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		5.0f, -5.0f, -5.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		5.0f,  5.0f, -5.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		5.0f, -5.0f, -5.0f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		5.0f,  5.0f,  5.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		5.0f, -5.0f,  5.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	};
	float surfaceVer2[] = {
		-25,  -2, -25,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		25,  -2,  25,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		25,  -2, -25,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		25,  -2,  25,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-25,  -2, -25,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-25,  -2,  25,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	};
	Render::addVBO(surfaceVer, 288, "wall");
	Render::addVBO(surfaceVer2, 288, "ground");
	Render::addVAO(pos, size, shift, 3, "wall", "wall");
	Render::addVAO(pos, size, shift, 3, "ground", "ground");
	Render::addTexture("./wall.jpg", "wall");
	Render::addTexture("./background.jpg", "background");
	//ResourceManager::loadResource("./resource.rec");
	//wall
	Entity *wall = new Entity(6);
	wall->VAO = "wall";
	wall->VBO = "wall";
	wall->texture = "wall";
	wall->shader = "texture";
	wall->rigid._is_static = true;
	wall->rigid.data.position = glm::vec3(0.0f, 0.0f, 0.0f);
	wall->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	wall->rigid._mass = 300000000;
	wall->rigid._restitution_coeffient = 1;
	wall->rigid.type = RECTANGLE;
	vector<glm::vec3> rigidVer;
	glm::vec3 ver1 = glm::vec3(5.0f, 5.0f, 5.0f);
	glm::vec3 ver2 = glm::vec3(5.0f, 5.0f, -5.0f);
	glm::vec3 ver3 = glm::vec3(5.0f, -5.0f, -5.0f);
	glm::vec3 ver4 = glm::vec3(5.0f, -5.0f, 5.0f);
	rigidVer.push_back(ver1);
	rigidVer.push_back(ver2);
	rigidVer.push_back(ver3);
	rigidVer.push_back(ver4);
	wall->rigid.setVertex(rigidVer);
	wall->rigid.setAABB();
	ObjectManager::object_list.push_back(wall);
	ObjectManager::aabb_tree.add(wall);
	Entity *wall2 = new Entity(6);
	wall2->VAO = "wall";
	wall2->VBO = "wall";
	wall2->texture = "wall";
	wall2->shader = "texture";
	wall2->rigid._is_static = true;
	wall2->rigid.data.position = glm::vec3(-10.0f, 0.0f, 0.0f);
	wall2->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	wall2->rigid._mass = 300000000;
	wall2->rigid._restitution_coeffient = 1;
	wall2->rigid.type = RECTANGLE;
	rigidVer.clear();
	ver1 = glm::vec3(-5.0f, 5.0f, 5.0f);
	ver2 = glm::vec3(-5.0f, 5.0f, -5.0f);
	ver3 = glm::vec3(-5.0f, -5.0f, -5.0f);
	ver4 = glm::vec3(-5.0f, -5.0f, 5.0f);
	rigidVer.push_back(ver1);
	rigidVer.push_back(ver2);
	rigidVer.push_back(ver3);
	rigidVer.push_back(ver4);
	wall2->rigid.setVertex(rigidVer);
	wall2->rigid.setAABB();
	ObjectManager::object_list.push_back(wall2);
	ObjectManager::aabb_tree.add(wall2);
	//wall
	Entity *ground = new Entity(6);
	ground->VAO = "ground";
	ground->VBO = "ground";
	ground->texture = "background";
	ground->shader = "texture";
	ground->rigid._is_static = true;
	ground->rigid.data.position = glm::vec3(0.0f, 0.0f, 0.0f);
	ground->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	ground->rigid._mass = 300000000;
	ground->rigid._restitution_coeffient = 0.5;
	ground->rigid.type = RECTANGLE;
	vector<glm::vec3> groundVer;
	glm::vec3 Ver1 = glm::vec3(25.0f, -2.0f, 25.0f);
	glm::vec3 Ver2 = glm::vec3(25.0f, -2.0f, -25.0f);
	glm::vec3 Ver3 = glm::vec3(-25.0f, -2.0f, -25.0f);
	glm::vec3 Ver4 = glm::vec3(-25.0f, -2.0f, 25.0f);
	groundVer.push_back(Ver1);
	groundVer.push_back(Ver2);
	groundVer.push_back(Ver3);
	groundVer.push_back(Ver4);
	ground->rigid.setVertex(groundVer);
	ground->rigid.setAABB();
	ObjectManager::object_list.push_back(ground);
	ObjectManager::aabb_tree.add(ground);
	//surface
	//

	Render::shaders.search("texture").use();
	Render::shaders.search("texture").setInt("material.diffuse", 0);

	//how to get an object's type
	//cout << typeid(player).name() << endl;


	int lastClock = clock();
	int count = 0;
	//////////////////////////////////////	start of the main loop	//////////////////////////////////////
	while (!glfwWindowShouldClose(sc.window)) {
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

		processInput(sc);
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// start to rending

		//test
		if(is_player)
			player->rigid.data.velocity = camera._v;
		//

		game.update(dt);

		//test
		if (is_player) {
			camera.Position = player->rigid.data.position - camera.radius * camera.Front;
			camera._v = player->rigid.data.velocity;
		}
		else {
			camera.Position += camera._v * dt;
		}
		//

		game.render();

		//test
		//aabbDebuger();
		showVersion();
		game.mouseCallback(sc.window, mouse_x, mouse_y);
		game.input(sc, dt);
		game.mouseButtonCallback(is_mouse_press);
		//

		//cout << camera.Position.x << "\t" << camera.Position.y << "\t" << camera.Position.z << endl;

		// end of loop
		while (1) {//控制FPS
			if (clock() - currentClock > 0) {
				break;
			}
		}
		//////////////////////////////////////	end of the main loop	//////////////////////////////////////
		glfwSwapBuffers(sc.window);
		glfwPollEvents();
	}


	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;


	//	system("PAUSE");
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE) {

	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, dt);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.processKeyboard(SPACE, dt);
}