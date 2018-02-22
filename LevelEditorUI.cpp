#include "LevelEditorUI.h"

GroundUI::GroundUI(float x, float y, float z)
{
	setXY(x, y);
	setZ(z);
}

void GroundUI::setXY(float x, float y)
{
	float xpos = x / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) + camera.getOrthoIntervalX1X2Y1Y2().x;
	float ypos = y / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) + camera.getOrthoIntervalX1X2Y1Y2().z;
	glm::vec4 pos = glm::vec4(xpos, ypos, 0, 1);
	pos = camera.getViewMatrix()*pos;
	_position = glm::vec2(pos.x, pos.y);
}

void GroundUI::setHeight(float height)
{
	float h = height / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) + camera.getOrthoIntervalX1X2Y1Y2().z;
	_height_width.x = h;
}

void GroundUI::setWidth(float width)
{
	float w = width / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) + camera.getOrthoIntervalX1X2Y1Y2().x;
	_height_width.y = w;
}

StairUI::StairUI(float x, float y, float z)
{
	setXY(x, y);
	setZ(z + 2.5);			//視實際情況調整
}

void StairUI::setXY(float x, float y)
{
	float xpos = x / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) + camera.getOrthoIntervalX1X2Y1Y2().x;
	float ypos = y / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) + camera.getOrthoIntervalX1X2Y1Y2().z;
	glm::vec4 pos = glm::vec4(xpos, ypos, 0, 1);
	pos = camera.getViewMatrix()*pos;
	_position = glm::vec2(pos.x, pos.y);
}

void StairUI::setHeight(float height)
{
	float h = height / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) + camera.getOrthoIntervalX1X2Y1Y2().z;
	_height_width.x = h;
}

void StairUI::setWidth(float width)
{
	float w = width / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) + camera.getOrthoIntervalX1X2Y1Y2().x;
	_height_width.y = w;
}

WallUI::WallUI(float x, float y, float z)
{
	setXY(x, y);
	setZ(z);
	_draw_type = GL_LINES;
	_vao = "line_vao";
}

void WallUI::setXY(float x, float y)
{
	float xpos = x / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) + camera.getOrthoIntervalX1X2Y1Y2().x;
	float ypos = y / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) + camera.getOrthoIntervalX1X2Y1Y2().z;
	glm::vec4 pos = glm::vec4(xpos, ypos, 0, 1);
	pos = camera.getViewMatrix()*pos;
	_position = glm::vec2(pos.x, pos.y);
}

void WallUI::setHeight(float height)
{
	float h = height / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z) + camera.getOrthoIntervalX1X2Y1Y2().z;
	_height_width.x = h;
}

void WallUI::setWidth(float width)
{
	float w = width / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x) + camera.getOrthoIntervalX1X2Y1Y2().x;
	_height_width.y = w;
}
