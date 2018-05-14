#pragma once
#include "UI.h"
#include "Include.h"
#include "Camera.h"
#include "Render.h"

extern Camera camera;

class BackgroundUI :public UI {
public:
	BackgroundUI(float x, float y, float z);

	virtual void hoverIn();
	virtual void hoverOut();

	virtual float x();
	virtual float y();
	virtual float width();
	virtual float height();

	virtual void draw();
};

class WallUI :public UI {
public:
	WallUI(glm::vec3 point1);				//
	virtual void setXY(float x, float y);
	virtual void setHeight(float height);
	virtual void setWidth(float width);
	virtual void setPoint1(glm::vec3 point);
	virtual void setPoint2(glm::vec3 point);

	virtual void hoverIn();
	virtual void hoverOut();

	virtual float x();
	virtual float y();
	virtual float width();
	virtual float height();

	virtual void draw();

	glm::vec3 _point1;		
	glm::vec3 _point2;		
};

