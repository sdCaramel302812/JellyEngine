#include "UI.h"



UI::UI()
{
	_id = createID();
	_callback_function = nullptr;
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

void UI::setCallback(CALL_BACK & callback)
{
	_callback_function = callback;
}

void UI::callback()
{
	if (_callback_function) {
		_callback_function();
	}
	else {
		std::cerr << "ERROR::CALLBACK_FUNCTION_NOT_FIND" << std::endl;
	}
}

int UI::_ui_num = 0;

int UI::createID()
{
	_ui_num++;
	return _ui_num;
}
