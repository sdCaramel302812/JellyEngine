#pragma once
#include <vector>
#include <string>
#include "Include.h"

class FrameAnimationData {
public:
	FrameAnimationData(std::string texture, glm::vec2 _left_down, glm::vec2 right_top);
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

	FrameAnimationData &&animation(float dt);

	void setOneTexture(bool value);
	bool isOneTexture();

private:
	int _fps;
	int _total_frame;
	int _width;
	int _height;
	int _current_frame = 0;
	float _time_cycle = 0;
	bool _one_texture = true;
	std::vector<std::string> _frame_texture;
};

