#ifndef SHADER_H
#define SHADER_H
#pragma once
#include <GL\glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


class Shader
{
public:
	Shader(const GLchar *vertexPath,const GLchar *fragmentPath);
	~Shader();
	void use();
	void setBool(const std::string &name,bool value)const;
	void setBool2(const std::string &name, bool value, bool value2)const;
	void setBool3(const std::string &name, bool value, bool value2, bool value3)const;
	void setBool4(const std::string &name, bool value, bool value2, bool value3, bool value4)const;
	void setInt(const std::string &name,int value)const;
	void setInt2(const std::string &name, int value, int value2)const;
	void setInt3(const std::string &name, int value, int value2, int value3)const;
	void setInt4(const std::string &name, int value, int value2, int value3, int value4)const;
	void setFloat(const std::string &name,float value)const;
	void setFloat2(const std::string &name, float value, float value2)const;
	void setFloat3(const std::string &name, float value, float value2, float value3)const;
	void setFloat4(const std::string &name, float value, float value2, float value3, float value4)const;
	void setMat4(const std::string &name, glm::mat4 matrix)const;
//private:
	unsigned int ID;
};

#endif