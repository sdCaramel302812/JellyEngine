#include "LevelEditorUI.h"

BackgroundUI::BackgroundUI(float x, float y, float z)
{
	_has_hover = true;
	_editor = true;
	setXY(x, 0);
	setZ(z);
}


void BackgroundUI::hoverIn()
{
}

void BackgroundUI::hoverOut()
{
}

float BackgroundUI::x()
{
	glm::vec4 pos = glm::vec4(_position.x, 0, getZ(), 1);
	pos = glm::inverse(glm::translate(glm::mat4(), camera.Position))*pos;
	float x = (pos.z - camera.getOrthoIntervalX1X2Y1Y2().x) * 1920 / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x);

	return x;
}

float BackgroundUI::y()
{
	glm::vec4 pos = glm::vec4(_position.y, 0, getZ(), 1);
	pos = glm::inverse(glm::translate(glm::mat4(), camera.Position))*pos;
	float y = (pos.x - camera.getOrthoIntervalX1X2Y1Y2().z) * 1920 / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z);

	return y;
}

float BackgroundUI::width()
{
	return  _height_width.y * 1920 / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x);
}

float BackgroundUI::height()
{
	return _height_width.x * 1080 / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z);
}

void BackgroundUI::draw()
{
	//Render::draw(this, 0);
}

WallUI::WallUI(glm::vec3 point1)		//real position in the map
{
	point1.y += 1;
	_has_hover = true;
	_editor = true;
	setColor(glm::vec4(0.6, 0.6, 1.0, 0.5));
	setXY(point1.x, 1);
	setZ(point1.z);
	setPoint1(point1);
	_vao = "line_vao";
	_draw_type = GL_LINES;
}

void WallUI::setXY(float x, float y)
{
	_position = glm::vec2(x, y);
}

void WallUI::setHeight(float h)
{
	_height_width.x = h;
}

void WallUI::setWidth(float w)
{
	_height_width.y = w;
}

void WallUI::setPoint1(glm::vec3 point)
{
	_point1 = point;
}

void WallUI::setPoint2(glm::vec3 point)
{
	_point2 = point;
	setHeight(_point2.x - _point1.x);
	setWidth(_point2.z - _point1.z);
}

void WallUI::hoverIn()
{
	setColor(glm::vec4(0.6, 0.6, 1.0, 1.0));
}

void WallUI::hoverOut()
{
	setColor(glm::vec4(0.6, 0.6, 1.0, 0.5));
}

float WallUI::x()
{
	glm::vec4 pos = glm::vec4(_point1.x, 0, _point1.z, 1);
	pos = glm::inverse(glm::translate(glm::mat4(), camera.Position))*pos;
	float x = (pos.z - camera.getOrthoIntervalX1X2Y1Y2().x) * 1920 / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x);
	
	return x;
}

float WallUI::y()
{
	glm::vec4 pos = glm::vec4(_point1.x, 0, _point1.z, 1);
	pos = glm::inverse(glm::translate(glm::mat4(), camera.Position))*pos;
	float y = (pos.x - camera.getOrthoIntervalX1X2Y1Y2().z) * 1080 / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z);

	return y;
}

float WallUI::width()
{
	return  _height_width.y * 1920 / (camera.getOrthoIntervalX1X2Y1Y2().y - camera.getOrthoIntervalX1X2Y1Y2().x);
}

float WallUI::height()
{
	return _height_width.x * 1080 / (camera.getOrthoIntervalX1X2Y1Y2().w - camera.getOrthoIntervalX1X2Y1Y2().z);
}

void WallUI::draw()
{
	_point2.y = _point1.y;
	float length = glm::length(_point2 - _point1);
	float radius = atan2(_point2.x - _point1.x, _point2.z - _point1.z);
	Render::drawLine(_point1, length, radius, _color, 0);
}
