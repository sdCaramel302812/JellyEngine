#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>
#include "Character.h"
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "Render.h"
#include <cstdlib>
using std::vector;
using std::string;
using std::ifstream;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static void loadResource(char *fileName);

	static void loadMap(char *fileName);
	static void loadMap(char *fileName, int editor);
	static void saveMap(char *fileName);

private:
	static string _path_name;
	static string _file_name;
};

