#pragma once
#include "UI.h"
#include "Camera.h"

extern Camera camera;

class GroundUI :public UI {
public:
	GroundUI(float x, float y, float z);
	virtual void setXY(float x, float y);
	virtual void setHeight(float height);
	virtual void setWidth(float width);
};

class StairUI :public UI {
public:
	StairUI(float x, float y, float z);
	virtual void setXY(float x, float y);
	virtual void setHeight(float height);
	virtual void setWidth(float width);
};

class WallUI :public UI {
public:
	WallUI(float x, float y, float z);
	virtual void setXY(float x, float y);
	virtual void setHeight(float height);
	virtual void setWidth(float width);
};

