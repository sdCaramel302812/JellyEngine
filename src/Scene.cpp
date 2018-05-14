#include "Scene.h"
#include <Imm.h>


Scene::Scene(int wid, int hei)
{
	sc_height = hei;
	sc_width = wid;
	int glfwInitCheck = glfwInit();
	if (glfwInitCheck == GLFW_FALSE)
	{
//		std::cout << "glfw initilization failed." << std::endl;
		//return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(wid, hei, "Jelly 2D", NULL, NULL);// glfwGetPrimaryMonitor() to be full screen
	full_sc = false;
	if (window == nullptr) // window creation failed
	{
		//return -1;
	}
	glfwMakeContextCurrent(window);
	// Set GLFW callback functions
	//	glfwSetErrorCallback(error_callback);
	//	glfwSetKeyCallback(window, key_callback);

	// According to source code, this make you to access successfully full extension from some latest driver without error.
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		//return -1;
	}

	if (ImmIsIME(GetKeyboardLayout(0))) {
		ImmSimulateHotKey(FindWindow(NULL,"Jelly 2D"), IME_KHOTKEY_ENGLISH);
	}
	// Define the viewport dimensions (if you will change the winodw size, put these in loop)
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	setVSync(true);

	//set cursor disabled
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Test ts;
	HWND hWnd = GetConsoleWindow(); //隱藏視窗1
	//ShowWindow(hWnd, SW_HIDE); //隱藏視窗2
							   // Looping Here until user trigger closing window event.
	
}


Scene::~Scene()
{
}

void Scene::setVSync(bool value)
{
	typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
	PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

	_vsync = value;
	if (value) {
		wglSwapIntervalEXT(1);//打開垂直同布
	}
	else {
		wglSwapIntervalEXT(0);//關閉垂直同布
	}
}

void Scene::setFullScreen(bool value)
{
	if (value) {
		/*
		glfwMaximizeWindow(window);
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		*/
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, sc_width, sc_height, 60);
		full_sc = true;
	}
	else {
		glfwSetWindowMonitor(window, NULL, 200, 200, sc_width, sc_height, 60);
		full_sc = false;
	}
}

bool Scene::isFullScreen()
{
	return full_sc;
}

bool Scene::isVSync()
{
	return _vsync;
}

void Scene::setSceneSize(int width, int height)
{
	sc_height = height;
	sc_width = width;
	if (full_sc) {
		glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, sc_width, sc_height, 60);
	}
	else {
		glfwSetWindowMonitor(window, NULL, 200, 200, sc_width, sc_height, 60);
	}
}

void Scene::setWindowName(std::string value)
{
	glfwSetWindowTitle(window, value.c_str());
}

void Scene::setCursorEnable(bool value)
{
	if (value) {
		glfwSetInputMode(window, GLFW_CURSOR,GLFW_CURSOR_NORMAL);
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

int Scene::getSceneWidth()
{
	return sc_width;
}

int Scene::getSceneHeight()
{
	return sc_height;
}
