#pragma once
#include <vector>
#include <string>
#include "Include.h"
#include <iostream>

class FrameAnimationData {
public:
	FrameAnimationData();
	FrameAnimationData(std::string texture, glm::vec2 _left_down, glm::vec2 right_top);
	void setData(std::string texture, glm::vec2 _left_down, glm::vec2 right_top);
	std::string _texture;
	glm::vec2 _point1;
	glm::vec2 _point2;
};

class FrameAnimation
{
public:
	FrameAnimation();
	FrameAnimation(int fps, int total_frame);
	FrameAnimation(int fps, int total_frame, int width, int height);		//width : number of pictures in a row, height : number of pictures in a column
	~FrameAnimation();

	void setFPS(int fps);
	int getFPS();

	void setTexture(std::string texture, int frame = 0);
	std::vector<std::string> &getTexture();

	FrameAnimationData &animated(float dt);
	FrameAnimationData _data;

	void setSize(int width, int height);
	void setOneTexture(bool value);
	bool isOneTexture();

	void setStartPoint(int value);
	void setStopPoint(int value);

private:
	int _fps;
	int _total_frame;
	int _width;
	int _height;
	int _current_frame = 0;
	int _start_point = -1;
	int _stop_point =-1;		//for some animation that has few action in one picture
	float _time_cycle = 0;
	bool _one_texture = true;
	std::vector<std::string> _frame_texture;
};

