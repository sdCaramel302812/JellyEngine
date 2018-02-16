#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <functional>
#include <iostream>
#include <string>
using std::string;

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

	void setCallback(const CALL_BACK &callback);
	void callback();
	bool hasCallback();

	void setHover(const CALL_BACK &hover);
	void hover();
	bool hasHover();

	virtual void draw();

	bool visable();
	void setVisable(bool value);

	int _id;

	void setZ(int z);
	int getZ();

	string _texture = "";
	glm::vec3 _color = glm::vec3(0.0f, 0.0f, 0.0f);
	GLenum _draw_type = GL_TRIANGLES;
	void setTexture(string texture);
	void setColor(glm::vec3 color);
	void setDrawType(GLenum type);

private:
	bool _visable = true;
	int _z_value = 1;
	CALL_BACK _callback_function;
	CALL_BACK _hover_event;
	bool _has_hover = false;
	bool _has_callback = false;
	glm::vec2 _position;
	glm::vec2 _height_width;
	static int createID();
	static int _ui_num;
};

