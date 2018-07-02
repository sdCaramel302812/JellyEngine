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

enum UIType {
	UI_BASE,
	WALL_UI,
};

class UI
{
public:
	typedef std::function<void(void)> CALL_BACK;
	typedef std::function<void(UI *)> HOVER_CALL_BACK;

	UI();
	UI(float x, float y, float width, float height);
	~UI();
	virtual float x();
	virtual float y();
	virtual float height();
	virtual float width();
	virtual void setXY(float x, float y);
	virtual void setHeight(float height);
	virtual void setWidth(float width);

	virtual void setPoint1(glm::vec3 point);
	virtual void setPoint2(glm::vec3 point);

	void setCallback(const CALL_BACK &callback);
	virtual void callback();
	bool hasCallback();

	void setHoverIn(const HOVER_CALL_BACK &hover);
	void setHoverOut(const HOVER_CALL_BACK &hover);
	virtual void hoverIn();
	virtual void hoverOut();
	bool hasHover();

	virtual void wheelEvent(float yoffset);
	bool hasWheelEvent();

	virtual void draw();

	bool visable();
	virtual void setVisable(bool value);
	bool enable();
	virtual void setEnable(bool value);

	int _id;

	bool _editor = false;		//if true, then use the camera projection

	void setZ(float z);
	float getZ();

	string _texture = "";
	string _texture2 = "";
	string _vao = "ui_vao";

	glm::vec4 _color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	GLenum _draw_type = GL_TRIANGLES;
	void setTexture(string texture);
	void setColor(glm::vec4 color);
	void setColor(float r, float g, float b, float a);
	void setDrawType(GLenum type);

	bool selected();
	virtual void setSelect(bool value);

	UIType _ui_type = UI_BASE;


	glm::vec4 _texure_cood;
	glm::vec2 _position;
	glm::vec2 _height_width;
protected:
	bool _visable = true;
	bool _enable = true;
	bool _is_selected = false;
	float _z_value = 0;
	CALL_BACK _callback_function;
	HOVER_CALL_BACK _hover_in;
	HOVER_CALL_BACK _hover_out;
	bool _has_hover = false;
	bool _has_callback = false;
	bool _has_wheel_event = false;
	
	static int createID();
	static int _ui_num;
};

