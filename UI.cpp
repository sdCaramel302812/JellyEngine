#include "UI.h"



UI::UI()
{
	_id = createID();
	_callback_function = nullptr;
	_hover_in = nullptr;
	_hover_out = nullptr;
}

UI::UI(float x, float y, float width, float height) :UI()
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

void UI::setPoint1(glm::vec3 point)
{
}

void UI::setPoint2(glm::vec3 point)
{
}

void UI::setCallback(const CALL_BACK & callback)
{
	_callback_function = callback;
	_has_callback = true;
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

bool UI::hasCallback()
{
	return _has_callback;
}

void UI::setHoverIn(const HOVER_CALL_BACK & hover)
{
	_hover_in = hover;
	_has_hover = true;
}

void UI::setHoverOut(const HOVER_CALL_BACK & hover)
{
	_hover_out = hover;
	_has_hover = true;
}

void UI::hoverIn()
{
	if (_hover_in) {
		_hover_in(this);
	}
	else {
		std::cerr << "ERROR::HOVER_EVENT_FUNCTION_NOT_FIND" << std::endl;
	}
}

void UI::hoverOut()
{
	if (_hover_out) {
		_hover_out(this);
	}
	else {
		std::cerr << "ERROR::HOVER_EVENT_FUNCTION_NOT_FIND" << std::endl;
	}
}

bool UI::hasHover()
{
	return _has_hover;
}

void UI::wheelEvent(float yoffset)
{
}

bool UI::hasWheelEvent()
{
	return _has_wheel_event;
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

bool UI::enable()
{
	return _enable;
}

void UI::setEnable(bool value)
{
	_enable = value;
}

void UI::setZ(float z)
{
	_z_value = z;
}

float UI::getZ()
{
	return _z_value;
}

void UI::setTexture(string texture)
{
	_texture = texture;
}

void UI::setColor(glm::vec4 color)
{
	_color = color;
}

void UI::setColor(float r, float g, float b, float a)
{
	_color = glm::vec4(r, g, b, a);
}

void UI::setDrawType(GLenum type)
{
	_draw_type = type;
}

bool UI::selected()
{
	return _is_selected;
}

void UI::setSelect(bool value)
{
	_is_selected = value;
}

int UI::_ui_num = 0;

int UI::createID()
{
	_ui_num++;
	return _ui_num;
}
