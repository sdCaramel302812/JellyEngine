#include "Render.h"

hash<unsigned int> Render::VAOs = hash<unsigned int>();
hash<unsigned int> Render::VBOs = hash<unsigned int>();
hash<unsigned int> Render::EBOs = hash<unsigned int>();
hash<unsigned int> Render::textures = hash<unsigned int>();
hash<Shader> Render::shaders = hash<Shader>();
std::map<wchar_t, Character> Render::Characters;

