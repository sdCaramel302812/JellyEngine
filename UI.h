#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <functional>
#include <iostream>

class UI
{
public:
	typedef std::function<void(void)> CALL_BACK;

	UI();
	UI(float x, float y, float height, float width);
	~UI();
	float x();
	float y();
	float height();
	float width();
	void setXY(float x, float y);
	void setHeight(float height);
	void setWidth(float width);

	void setCallback(CALL_BACK &callback);
	void callback();

	void setHover(CALL_BACK &hover);
	void hover();

	int _id;

	void setZ(int z);
	int getZ();

private:
	int _z_value = 1;
	CALL_BACK _callback_function;
	CALL_BACK _hover_event;
	glm::vec2 _position;
	glm::vec2 _height_width;
	static int createID();
	static int _ui_num;
};

