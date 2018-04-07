#pragma once

#define SOFTWARE_VERSION "ver 0.5.0"

//	Windows
#define _WIN32_WINNT 0x0500
//	Windows

//	OpenGL
#define GLEW_STATIC
//	OpenGL

//	stb image
#define STB_IMAGE_IMPLEMENTATION
//	stb image

//	Scene
#define SCENE_WIDTH 1280
#define SCENE_HEIGHT 720
//	Scene

//	Physics
#define MAX_DT 0.032		//to avoid too low fps
#define MIN_DT 0.001		//to avoid too high fps
#define TERMINAL_SPEED 150	//max falling speed
#define ALLOW_DIST 0.0001	//allow two entity overlaping length
//	Physics

#define CONTAIN_LIMIT 0.1	//tolerent length of contain an aabb 

#include <functional>
typedef std::function<void(void)> CALL_BACK;
		