// GLEW ( help you using functions without retreiving functions )
#define _WIN32_WINNT 0x0500
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
//#include "Model.h"

// Get error from GLFW for debuging
//void error_callback(int error, const char* description);
// Get keywords from GLFW windows
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);


static GLint fogMode;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

TPCamera TPcamera(cameraPos, cameraUp);
float dt = 0;
float last_frame = 0;

Camera2D camera2d(cameraPos);

void processInput(Scene &sc)
{
	if (glfwGetKey(sc.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(sc.window, true);
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_RELEASE) {

	}
	if (glfwGetKey(sc.window, GLFW_KEY_W) == GLFW_PRESS)
		TPcamera.processKeyboard(FORWARD, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_S) == GLFW_PRESS)
		TPcamera.processKeyboard(BACKWARD, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_A) == GLFW_PRESS)
		TPcamera.processKeyboard(LEFT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_D) == GLFW_PRESS)
		TPcamera.processKeyboard(RIGHT, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_SPACE) == GLFW_PRESS)
		TPcamera.processKeyboard(SPACE, dt);
	if (glfwGetKey(sc.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		if (sc.isFullScreen()) {
			sc.setFullScreen(false);
		}
		else {
			sc.setFullScreen(true);
		}

	}
}

bool firstMouse = true;
float lastX = 1080 / 2, lastY = 760 / 2;

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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
	TPcamera.processMouseEvent(xoffset, yoffset);
}

//typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
//PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;



int main() {
	Scene sc;

	//Shader shader("./vertex.vs", "./fragment.fs");
	Shader lampShader("./lamp.vs", "./lamp.fs");
	//Shader groundShader("./vertex.vs", "./ground.fs");

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);


	glfwSetInputMode(sc.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(sc.window, mouse_callback);

	//wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
	//wglSwapIntervalEXT(0);
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 5.0f, -1.0f),
		glm::vec3(1.0f, 10.0f, -3.0f),
		glm::vec3(2.0f, 15.0f, -5.0f),
		glm::vec3(3.0f, 20.0f, -7.0f),
		glm::vec3(4.0f, 25.0f, -9.0f)
	};


	//封裝測試
	Game game;

	Render::addVBO(vertices, 288, "box");
	int pos[3] = { 0,1,2 };
	int size[3] = { 3,2,3 };
	int shift[3] = { 0,5,3 };
	Render::addVAO(pos, size, shift, 3, "box", "box");
	Render::addTexture("./container.png", "box");
	Render::addShader("box", "./rtest.vs", "./rtest.fs");

	for (int i = 0; i < 5; ++i) {
		Entity *box = new Entity(36);
		box->VAO = "box";
		box->VBO = "box";
		box->texture = "box";
		box->shader = "box";
		box->rigid._is_static = false;
		box->rigid.type = NO_COLLIDE;
		box->rigid._mass = 10;
		box->rigid.data.position = cubePositions[i];
		box->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		ObjectManager::object_list.push_back(box);
		//cout << box->rigid.data.position.y << endl;

	}

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
		Render::addShader("sphere", "./lamp.vs", "./lamp.fs");
		Entity *sphere = new Entity(1944);
		sphere->VAO = "sphere";
		sphere->VBO = "sphere";
		//sphere->EBO = "sphere";
		sphere->shader = "sphere";
		sphere->rigid._is_static = false;
		sphere->rigid.data.position = glm::vec3(0.0f, 25.0f, 0.0f);
		sphere->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		sphere->rigid.setSphereRadius(0.5);
		sphere->rigid._mass = 10;
		sphere->rigid._restitution_coeffient = 0.8;
		sphere->rigid.type = SPHERE;
		//ObjectManager::object_list.push_back(sphere);
		Entity *sphere1 = new Entity(1944);
		sphere1->VAO = "sphere";
		sphere1->VBO = "sphere";
		//sphere->EBO = "sphere";
		sphere1->shader = "sphere";
		sphere1->rigid._is_static = false;
		sphere1->rigid.data.position = glm::vec3(0.0f, 10.0f, 0.5f);
		sphere1->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		sphere1->rigid.setSphereRadius(0.5);
		sphere1->rigid._mass = 300;
		sphere1->rigid._restitution_coeffient = 0.8;
		sphere1->rigid.type = SPHERE;
		ObjectManager::object_list.push_back(sphere1);
		
	}
	//sphere

	Player *player = new Player(1944);
	player->rigid.setSphereRadius(0.5);
	player->rigid._mass = 50;
	player->VAO = "sphere";
	player->VBO = "sphere";
	//sphere->EBO = "sphere";
	player->shader = "sphere";
	player->rigid.data.position = glm::vec3(0.0f, 11.0f, 0.5f);
	//player->rigid.data.position = TPcamera.Position + TPcamera.radius * TPcamera.Front;
	player->rigid._restitution_coeffient = 0.8;
	player->rigid.type = SPHERE;
	player->rigid._is_static = false;
	player->e_type = PLAYER;
	ObjectManager::object_list.push_back(player);
	
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
	Render::addTexture("./grass.jpg", "grass");
	Render::addShader("wall", "./rtest.vs", "./rtest.fs");
	Entity *wall = new Entity(6);
	wall->VAO = "wall";
	wall->VBO = "wall";
	wall->texture = "wall";
	wall->shader = "wall";
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
	wall->rigid.setRectangleVertex(rigidVer);
	ObjectManager::object_list.push_back(wall);
	Entity *ground = new Entity(6);
	ground->VAO = "ground";
	ground->VBO = "ground";
	ground->texture = "grass";
	ground->shader = "wall";
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
	ground->rigid.setRectangleVertex(groundVer);
	ObjectManager::object_list.push_back(ground);
	//surface
	//


	Render::shaders.search("box").use();
	Render::shaders.search("box").setInt("material.diffuse", 0);

	

	cout << typeid(player).name() << endl;

	int lastClock = clock();
	int count = 0;
	while (!glfwWindowShouldClose(sc.window)) {
		float current_frame = glfwGetTime();
		dt = current_frame - last_frame;
		last_frame = current_frame;
		int currentClock = clock();
		if (currentClock - lastClock < 1000) {
			++count;
		}
		else {
			lastClock = currentClock;
			std::cerr << "fps : " << count << endl;
			count = 0;
		}

		processInput(sc);
		glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// start to rending

		

		//player->rigid.data.position = camera.Position + camera.radius * camera.Front;
		//player->rigid.data.position.y = 0;
		player->rigid.data.velocity = TPcamera._v;
		//cout << player->rigid.data.position.x << " " << player->rigid.data.position.y << " " << player->rigid.data.position.z << endl;
		//cout << render.shaders.search("box").ID << endl;
		//Render::draw(*box, GL_TRIANGLES);
		//cout << camera.Position.y << endl;
		game.update(dt);

		TPcamera.Position = player->rigid.data.position - TPcamera.radius * TPcamera.Front;
		TPcamera._v = player->rigid.data.velocity;

		game.render();

		//if(count%100==0)
		//cout << player->rigid.data.velocity.x << " " << player->rigid.data.velocity.y << " " << player->rigid.data.velocity.z << endl;

		//camera._is_moving = false;


		// end of loop
		while (1) {//控制FPS
			if (clock() - currentClock > 0) {
				break;
			}
		}
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
		TPcamera.processKeyboard(FORWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		TPcamera.processKeyboard(BACKWARD, dt);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		TPcamera.processKeyboard(LEFT, dt);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		TPcamera.processKeyboard(RIGHT, dt);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		TPcamera.processKeyboard(SPACE, dt);
}