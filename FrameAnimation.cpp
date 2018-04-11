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
	_width = width;
	_height = height;
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

FrameAnimationData & FrameAnimation::animated(float dt)
{
	_time_cycle += dt;
	if (_time_cycle > 1.0 / _fps) {
		_time_cycle -= 1.0 / _fps;
		if (_current_frame < _total_frame && _stop_point == -1) {
			++_current_frame;
		}
		else if (_stop_point != -1 && _current_frame < _stop_point) {
			++_current_frame;
		}
		else if(_current_frame >= _total_frame && _stop_point == -1){
			_current_frame = 0;
		}
		else if (_stop_point != -1 && _current_frame >= _stop_point) {
			_current_frame = _start_point;
		}
	}
	if (_one_texture) {
		float x = 0, y = 0;
		x = _current_frame % _width / (float)_width;
		y = 1 - (( _current_frame / _height) / (float)_height) - 1 / (float)_height;
		_data.setData(_frame_texture.at(0), glm::vec2(x, y), glm::vec2((1 / (float)_width), (1 / (float)_height)));
		return _data;
	}
	else {
		_data.setData(_frame_texture.at(_current_frame), glm::vec2(0, 0), glm::vec2(1, 1));
		return _data;
	}
}

void FrameAnimation::setSize(int width, int height)
{
	_width = width;
	_height = height;
}

void FrameAnimation::setOneTexture(bool value)
{
	_one_texture = value;
}

bool FrameAnimation::isOneTexture()
{
	return _one_texture;
}

void FrameAnimation::setStartPoint(int value)
{
	_start_point = value;
}

void FrameAnimation::setStopPoint(int value)
{
	_stop_point = value;
}

FrameAnimationData::FrameAnimationData()
{
}

FrameAnimationData::FrameAnimationData(std::string texture, glm::vec2 _left_down, glm::vec2 _right_top)
{
	_texture = texture;
	_point1 = _left_down;
	_point2 = _right_top;
}

void FrameAnimationData::setData(std::string texture, glm::vec2 _left_down, glm::vec2 _right_top)
{
	_texture = texture;
	_point1 = _left_down;
	_point2 = _right_top;
}
