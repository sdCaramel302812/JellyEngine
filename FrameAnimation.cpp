#include "FrameAnimation.h"



FrameAnimation::FrameAnimation()
{
}

FrameAnimation::FrameAnimation(int fps, int total_frame)
{
	_fps = fps;
	_total_frame = total_frame;
}

FrameAnimation::FrameAnimation(int fps, int total_frame, int width, int height)
{
	_fps = fps;
	_total_frame = total_frame;
}


FrameAnimation::~FrameAnimation()
{
}

void FrameAnimation::setFPS(int fps)
{
	_fps = fps;
}

int FrameAnimation::getFPS()
{
	return _fps;
}

void FrameAnimation::setTexture(std::string texture, int frame)
{
	if (_frame_texture.empty()) {
		_frame_texture.push_back(texture);
	}
	else if (_frame_texture.size() >= frame) {
		_frame_texture.at(frame) = texture;
	}
	else {
		_frame_texture.push_back(texture);
	}
}

std::vector<std::string>& FrameAnimation::getTexture()
{
	return _frame_texture;
}

FrameAnimationData && FrameAnimation::animation(float dt)
{
	_time_cycle += dt;
	if (_time_cycle > 1 / _fps) {
		_time_cycle -= 1 / _fps;
		if (_current_frame < _total_frame) {
			++_current_frame;
		}
		else {
			_current_frame = 0;
		}
	}
	if (_one_texture) {
		float x, y;
		x = 1 / (_current_frame / _width);
		y = 1 - 1 / ( _current_frame % _height);
		FrameAnimationData data(_frame_texture.at(0), glm::vec2(x, y), glm::vec2(x + 1 / _width, y + 1 / _height));
		return std::move(data);
	}
	else {
		FrameAnimationData data(_frame_texture.at(_current_frame), glm::vec2(0, 0), glm::vec2(1, 1));
		return std::move(data);
	}
}

void FrameAnimation::setOneTexture(bool value)
{
	_one_texture = value;
}

bool FrameAnimation::isOneTexture()
{
	return _one_texture;
}

FrameAnimationData::FrameAnimationData(std::string texture, glm::vec2 _left_down, glm::vec2 _right_top)
{
	_texture = texture;
	_point1 = _left_down;
	_point2 = _right_top;
}
