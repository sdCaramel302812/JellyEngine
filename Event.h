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
protected:
	EventType _type;
	T _target_id;
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
		Render::draw(*_target_id, _draw_type);

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



#include "Event.cpp"

#endif

