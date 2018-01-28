 #ifndef SCENE_H
#define SCENE_H
#pragma once
#define _WIN32_WINNT 0x0500
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>
#include <iostream>
#include <Windows.h>
#include <Winuser.h>
#include <string>

class Scene
{
public:
	GLFWwindow* window;
	Scene(int width = 1280, int height = 720);
	~Scene();
	void setVSync(bool value);
	void setFullScreen(bool value);
	void setSceneSize(int width, int height);
	void setWindowName(std::string value);
	void setCursorEnable(bool value);
	bool isFullScreen();
	bool isVSync();
	int getSceneWidth();
	int getSceneHeight();
private:
	int sc_width;
	int sc_height;
	bool full_sc;
	bool _vsync;
};

#endif