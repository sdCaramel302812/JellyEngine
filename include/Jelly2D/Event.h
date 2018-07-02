#ifndef EVENT_H
#define EVENT_H
#include <iostream>
#include "Entity.h"
#include "Render.h"


enum EventType{
	NO_EVENT,
	MOTION_EVENT,
	DRAW_EVENT,
};


template<typename T>
class Event {
public:
	Event();
	EventType getType();
	void setTarget(T target);
	//virtual void use() = 0;
	T _target_id;
protected:
	EventType _type;
	
};

template<typename T>
class UpdateEvent : public Event<T> {
public:
	virtual void use();//對 event target 執行操作
};

template<typename T>
class RenderEvent : public Event<T> {
public:
	virtual void use() = 0;
};

//經過物理引擎處理後交給渲染引擎的資料
template<typename T>
class MotionEvent :public UpdateEvent<T> {
public:
	MotionEvent();
	void setDisplace(float x, float y, float z);
	void setDisplace(glm::vec3 pos);
	void plusDisplace(float x, float y, float z);
	void plusDisplace(glm::vec3 pos);
	glm::vec3 getDisplace();
	void setRotate(float angle, float x, float y, float z);
	void use();
	float dx();
	float dy();
	float dz();
	float angle();
	float rX();
	float rY();
	float rZ();
private:
	float _x = 0;
	float _y = 0;
	float _z = 0;
	float _rotate_angle = 0;
	float _rotate_x = 0;
	float _rotate_y = 0;
	float _rotate_z = 0;
};

template<typename T>
class DataEvent : public UpdateEvent<T> {
public:
	void use();
	void setV(glm::vec3 v);
	void plusV(glm::vec3 v);
//private:
	//速度
	glm::vec3 _d_v = glm::vec3(0.0f, 0.0f, 0.0f);
	//位置
	glm::vec3 _d_pos = glm::vec3(0.0f, 0.0f, 0.0f);
};

template<typename T>
class DrawEvent :public RenderEvent<T> {
public:
	DrawEvent()
	{
		_type = DRAW_EVENT;
		_draw_type = GL_TRIANGLES;
	}
	void use()
	{
		if (strncmp(typeid(_target_id).name(), "class Entity * ", 12) == 0) {
			Render::draw(_target_id, _draw_type);
		}
		if (strncmp(typeid(_target_id).name(), "class UI *", 8) == 0) {
				Render::draw(_target_id);
		}
		if (strncmp(typeid(_target_id).name(), "class TextItem *", 14) == 0) {
			Render::draw(_target_id);
		}

	}
	void setDrawType(GLenum type);
private:
	GLenum _draw_type;
};

template<typename T>
inline void UpdateEvent<T>::use()
{

}

template<typename T>
inline void DataEvent<T>::use()
{
	_target_id->rigid.data.velocity += _d_v;
}

template<typename T>
inline void DataEvent<T>::setV(glm::vec3 v)
{
	_d_v = v;
}

template<typename T>
inline void DataEvent<T>::plusV(glm::vec3 v)
{
	_d_v += v;
}



template<typename T>
MotionEvent<T>::MotionEvent()
{
	_type = MOTION_EVENT;
}

template<typename T>
void MotionEvent<T>::setDisplace(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

template<typename T>
void MotionEvent<T>::setDisplace(glm::vec3 pos)
{
	_x = pos.x;
	_y = pos.y;
	_z = pos.z;
}

template<typename T>
void MotionEvent<T>::plusDisplace(float x, float y, float z)
{
	_x += x;
	_y += y;
	_z += z;
}

template<typename T>
void MotionEvent<T>::plusDisplace(glm::vec3 pos)
{
	_x += pos.x;
	_y += pos.y;
	_z += pos.z;
}

template<typename T>
glm::vec3 MotionEvent<T>::getDisplace()
{
	return glm::vec3(_x, _y, _z);
}

template<typename T>
void MotionEvent<T>::setRotate(float angle, float x, float y, float z)
{
}

template<typename T>
void MotionEvent<T>::use()
{
	_target_id->rigid.changePosition(dx(), dy(), dz());
}

template<typename T>
float MotionEvent<T>::dx()
{
	return _x;
}

template<typename T>
float MotionEvent<T>::dy()
{
	return _y;
}

template<typename T>
float MotionEvent<T>::dz()
{
	return _z;
}

template<typename T>
float MotionEvent<T>::angle()
{
	return _rotate_angle;
}

template<typename T>
float MotionEvent<T>::rX()
{
	return _rotate_x;
}

template<typename T>
float MotionEvent<T>::rY()
{
	return _rotate_y;
}

template<typename T>
float MotionEvent<T>::rZ()
{
	return _rotate_z;
}


template<typename T>
Event<T>::Event()
{
}


template<typename T>
EventType Event<T>::getType()
{
	return _type;
}

/*template<typename T>
void Event<T>::use()
{
}*/

template<typename T>
void Event<T>::setTarget(T target)
{
	_target_id = target;
}

/*template<typename T>
void Event<T>::use()
{
}*/

template<typename T>
void DrawEvent<T>::setDrawType(GLenum type)
{
	_draw_type = type;
}

#endif

