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
#include "ObjectManager.h"
#include "Include.h"
#include <io.h>
#include "nstdlib.h"
#include "LevelEditorUI.h"
using std::vector;
using std::string;
using std::ifstream;

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	static void loadPrefabList(char *fileName);
	static void loadResource(char *fileName);
	static void loadMapList(char *dirName);

	static void loadMap(string fileName);
	static void loadMap(string fileName, int editor);
	static void saveMap(string fileName);

	static vector<string> &getPrefabList();
	static vector<string> &getImageList();
	static vector<string> &getMapList();

private:
	static vector<string> _prefab_list;
	static vector<string> _image_list;
	static vector<string> _map_list;
	static string _path_name;
	static string _file_name;
};

