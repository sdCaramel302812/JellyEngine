#include "UI.h"



UI::UI()
{
	_id = createID();
	_callback_function = nullptr;
	_hover_event = nullptr;
}

UI::UI(float x, float y, float height, float width) :UI()
{
	_position.x = x;
	_position.y = y;
	_height_width.x = height;
	_height_width.y = width;
}


UI::~UI()
{
}

float UI::x()
{
	return _position.x;
}

float UI::y()
{
	return _position.y;
}

float UI::height()
{
	return _height_width.x;
}

float UI::width()
{
	return _height_width.y;
}

void UI::setXY(float x, float y)
{
	_position.x = x;
	_position.y = y;
}

void UI::setHeight(float height)
{
	_height_width.x = height;
}

void UI::setWidth(float width)
{
	_height_width.y = width;
}

void UI::setCallback(const CALL_BACK & callback)
{
	_callback_function = callback;
	_has_callback = true;
}

void UI::callback()
{
	std::cout << "callback" << std::endl;
	if (_callback_function) {
		_callback_function();
	}
	else {
		std::cerr << "ERROR::CALLBACK_FUNCTION_NOT_FIND" << std::endl;
	}
}

bool UI::hasCallback()
{
	return _has_callback;
}

void UI::setHover(const CALL_BACK & hover)
{
	_hover_event = hover;
	_has_hover = true;
}

void UI::hover()
{
	if (_hover_event) {
		_hover_event();
	}
	else {
		std::cerr << "ERROR::HOVER_EVENT_FUNCTION_NOT_FIND" << std::endl;
	}
}

bool UI::hasHover()
{
	return _has_hover;
}

void UI::draw()
{
}

bool UI::visable()
{
	return _visable;
}

void UI::setVisable(bool value)
{
	_visable = value;
}

void UI::setZ(int z)
{
	_z_value = z;
}

int UI::getZ()
{
	return _z_value;
}

void UI::setTexture(string texture)
{
	_texture = texture;
}

void UI::setColor(glm::vec3 color)
{
	_color = color;
}

void UI::setDrawType(GLenum type)
{
	_draw_type = type;
}

int UI::_ui_num = 0;

int UI::createID()
{
	_ui_num++;
	return _ui_num;
}
