#include "Render.h"

std::unordered_map<string, unsigned int> Render::VAOs = std::unordered_map<string, unsigned int>();
std::unordered_map<string, unsigned int> Render::VBOs = std::unordered_map<string, unsigned int>();
std::unordered_map<string, unsigned int> Render::EBOs = std::unordered_map<string, unsigned int>();
std::unordered_map<string, unsigned int> Render::textures = std::unordered_map<string, unsigned int>();
std::unordered_map<string, Shader> Render::shaders = std::unordered_map<string, Shader>();
std::map<wchar_t, Character> Render::Characters;

