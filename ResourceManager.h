#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>
#include "Character.h"
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <iostream>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
	static void addGlyph(wchar_t c);

	static std::map<wchar_t, Character> Characters;
};

