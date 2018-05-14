#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <ctime>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPACE,
	SHIFT
};

// Default camera values
const float YAW = -90.0;
const float PITCH = 0.0;
const float SPEED = 10;
const float SENSITIVITY = 0.08;
const float ZOOM = 45.0;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL

class Camera
{
public:
	// Camera Attributes
	//	int count = 0;
	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	glm::vec3 Position;
	glm::vec3 _v;
	// Eular Angles
	float yaw;
	float pitch;
	float speed;
	float sensitivity;
	float zoom = 1;
	float radius = 5;
	bool _is_moving = false;
	bool _is_jumping = false;
	int _jump_cool_down;

	float _interval_x1 = -6.4;
	float _interval_x2 = 6.4;
	float _interval_y1 = -3.6;
	float _interval_y2 = 3.6;

	int _tilt_angle = 0;			//for sometime we want the camera to display in another angle
	float _view_distance = 5.5;		//for different angle should have different view distance

	int _camera_mode = 3;	// 1 : first person, 2 : 2D camera, 3 : third person

	/////////////////////////////////////////////////////////////////////////////////
	Camera(int mode = 3, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) : speed(SPEED), sensitivity(SENSITIVITY), zoom(1) {
		Position = position;
		_v = glm::vec3(0.0f, 0.0f, 0.0f);
		_camera_mode = mode;
		if (mode == 3) {
			WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
			Front = glm::vec3(0.0f, 0.0f, -1.0f);
			yaw = YAW + 90;
			pitch = PITCH;
			updateCamera();	
		}
		else if (mode == 2) {
			Position = glm::vec3(0.0f, 5.0f, 0.0f);
			WorldUp = glm::vec3(1.0f, 0.0f, 0.0f);
			Up = glm::vec3(1.0f, 0.0f, 0.0f);
			Front = glm::vec3(0.0f, -1.0f, 0.0f);
			Right = glm::vec3(0.0f, 0.0f, 1.0f);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float y = YAW, float p = PITCH) : Front(0.0f, 0.0f, -1.0f), speed(SPEED), sensitivity(SENSITIVITY), zoom(ZOOM) {
		Position.x = posX;
		Position.y = posY;
		Position.z = posZ;
		WorldUp.x = upX;
		WorldUp.y = upY;
		WorldUp.z = upZ;
		_v = glm::vec3(0.0f, 0.0f, 0.0f);
		yaw = y;
		pitch = p;
		updateCamera();
	}
	/////////////////////////////////////////////////////////////////////////////////
	~Camera() {}
	/////////////////////////////////////////////////////////////////////////////////
	void setAngle(int angle) {		//now just support 0 and 45 degree
		if (angle == 0) {
			Up = glm::vec3(1.0f, 0.0f, 0.0f);
			Front = glm::vec3(0.0f, -1.0f, 0.0f);
			_interval_y1 = -3.6;
			_interval_y2 = 3.6;
			_view_distance = 5.5;
			Position.y = 5.0;
		}
		else if (angle == 45) {
			Up = glm::vec3(0.7f, 0.7f, 0.0f);
			Front = glm::vec3(0.7f, -0.7f, 0.0f);
			_interval_y1 = -2.5;
			_interval_y2 = 2.5;
			_view_distance = 11.5;
			Position.y = 6.2;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 getViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}
	/////////////////////////////////////////////////////////////////////////////////
	glm::mat4 getUIViewMatrix() {
		return glm::lookAt(glm::vec3(), Front, Up);
	}
	/////////////////////////////////////////////////////////////////////////////////
	glm::mat4 getProjectMatrix() {
		if (_camera_mode == 2) {
			return glm::ortho(_interval_x1 * zoom, _interval_x2 * zoom, _interval_y1 * zoom, _interval_y2 * zoom, 0.1f, _view_distance);
		}
		else if (_camera_mode == 3) {
			return glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	glm::vec4 getOrthoIntervalX1X2Y1Y2() {
		return glm::vec4(_interval_x1 * zoom, _interval_x2 * zoom, _interval_y1 * zoom, _interval_y2 * zoom);
	}
	/////////////////////////////////////////////////////////////////////////////////
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void processKeyboard(CameraMovement direction, float delta_time) {
		float velocity = speed*delta_time;
		_is_moving = true;
		if (_camera_mode == 3) {		//third person mode
			if (direction == FORWARD) {
				if (glm::length(_v) < 20) {
					_v += glm::vec3(Front.x*velocity * 16, 0.0f, Front.z*velocity * 16);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(Front.x*velocity * 12, 0.0f, Front.z*velocity * 12);
				}
			}
			if (direction == BACKWARD) {
				if (glm::length(_v) < 20) {
					_v += glm::vec3(-Front.x*velocity * 16, 0.0f, -Front.z*velocity * 16);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(-Front.x*velocity * 12, 0.0f, -Front.z*velocity * 12);
				}
			}
			if (direction == LEFT) {
				if (glm::length(_v) < 20) {
					_v += glm::vec3(-Right.x*velocity * 16, 0.0f, -Right.z*velocity * 16);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(-Right.x*velocity * 12, 0.0f, -Right.z*velocity * 12);
				}
			}
			if (direction == RIGHT) {
				if (glm::length(_v) < 20) {
					_v += glm::vec3(Right.x*velocity * 16, 0.0f, Right.z*velocity * 16);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(Right.x*velocity * 12, 0.0f, Right.z*velocity * 12);
				}
			}
			if (direction == SPACE) {
				if (!_is_jumping) {
					_v += glm::vec3(0.0f, 5.0f, 0.0f);
					std::cerr << "space!!" << std::endl;
					_jump_cool_down = clock();
					_is_jumping = true;
				}
				if (clock() - _jump_cool_down > 300) {
					_is_jumping = false;
				}
			}
			if (direction == SHIFT) {
				if (!_is_jumping) {
					_v += glm::vec3(0.0f, -5.0f, 0.0f);
					std::cerr << "shift!!" << std::endl;
					_jump_cool_down = clock();
					_is_jumping = true;
				}
				if (clock() - _jump_cool_down > 300) {
					_is_jumping = false;
				}
			}
		}
		else if (_camera_mode == 2) {		//2D mode
			if (direction == FORWARD) {;
				if (glm::length(_v) < 15) {
					_v += glm::vec3(WorldUp.x*velocity * 12, WorldUp.y*velocity * 12, WorldUp.z*velocity * 12);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(WorldUp.x*velocity * 8, WorldUp.y*velocity * 8, WorldUp.z*velocity * 8);
				}
			}
			if (direction == BACKWARD) {
				if (glm::length(_v) < 15) {
					_v += glm::vec3(-WorldUp.x*velocity * 12, -WorldUp.y*velocity * 12, -WorldUp.z*velocity * 12);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(-WorldUp.x*velocity * 8, -WorldUp.y*velocity * 8, -WorldUp.z*velocity * 8);
				}
			}
			if (direction == LEFT) {
				if (glm::length(_v) < 15) {
					_v += glm::vec3(-Right.x*velocity * 12, 0.0f, -Right.z*velocity * 12);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(-Right.x*velocity * 8, 0.0f, -Right.z*velocity * 8);
				}
			}
			if (direction == RIGHT) {
				if (glm::length(_v) < 15) {
					_v += glm::vec3(Right.x*velocity * 12, 0.0f, Right.z*velocity * 12);
				}
				else {
					_v *= 0.5;
					_v += glm::vec3(Right.x*velocity * 8, 0.0f, Right.z*velocity * 8);
				}
			}
			if (direction == SPACE) {

			}
			if (direction == SHIFT) {

			}
		}
		//		std::cerr << "press keyboard" <<count<< std::endl;
		//		++count;
	}
	/////////////////////////////////////////////////////////////////////////////////
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void processMouseEvent(float xoffset, float yoffset, bool constrain_pitch = true) {
		if (_camera_mode == 3) {
			xoffset *= sensitivity;
			yoffset *= sensitivity;
			yaw += xoffset;
			pitch += yoffset;
			if (pitch > 89) {
				yoffset = yoffset - pitch + 89;
			}
			if (pitch < -89) {
				yoffset = yoffset - pitch - 89;
			}
			//先回到 world coordinate 旋轉再轉回 camera coordinate
			glm::vec3 basis = -(Position + radius * Front);
			glm::mat4 trans1, trans2, trans3;
			trans1 = glm::translate(trans1, basis);
			trans2 = glm::rotate(trans2, glm::radians(-xoffset), glm::vec3(0.0f, 1.0f, 0.0f));
			glm::vec3 front2d = glm::vec3(Front.x, 0.0f, Front.z);
			glm::vec3 rotate_axis = glm::cross(front2d, glm::vec3(0.0f, 1.0f, 0.0f));//上下旋轉的旋轉軸
			trans2 = glm::rotate(trans2, glm::radians(yoffset), rotate_axis);
			trans3 = glm::translate(trans3, -basis);
			Position = glm::vec3(trans3 * trans2 * trans1 * glm::vec4(Position, 1.0f));


			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrain_pitch) {
				if (pitch > 89) {
					pitch = 89;
				}
				if (pitch < -89) {
					pitch = -89;
				}
			}
			// Update Front, Right and Up Vectors using the updated Eular angles
			updateCamera();
		}
		else if (_camera_mode == 2) {

		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	float processScrollEvent(float yoffset) {
		if (_camera_mode == 3) {
			yoffset *= 5;
			if (zoom >= 10.0f && zoom <= 45.0f)
				zoom -= yoffset;
			if (zoom <= 10.0f)
				zoom = 10.0f;
			if (zoom >= 45.0f)
				zoom = 45.0f;
			return zoom;
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	void setCameraMode(int mode) {
		_camera_mode = mode;
	}
	/////////////////////////////////////////////////////////////////////////////////
	int getCameraMode() {
		return _camera_mode;
	}
	/////////////////////////////////////////////////////////////////////////////////
private:
	void updateCamera() {
		// Calculate the new Front vector
		if (_camera_mode == 3) {
			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			Front = glm::normalize(front);
			// Also re-calculate the Right and Up vector
			Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = glm::normalize(glm::cross(Right, Front));
		}
		else if (_camera_mode == 2) {

		}
	}
};




#endif