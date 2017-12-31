#ifndef EVENT_CPP
#define EVENT_CPP
#include "Event.h"



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
	_target_id->rigid.data.position.x += dx();
	_target_id->rigid.data.position.y += dy();
	_target_id->rigid.data.position.z += dz();
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

/*void EventBase::use()
{
}*/

#endif




