#include "Shader.h"



Shader::Shader(const GLchar *vertexPath, const GLchar *fragmentPath)
{
	int success;
	char infoLog[512];
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vFile;
	std::ifstream fFile;
	vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vFile.open(vertexPath);
		fFile.open(fragmentPath);
		std::stringstream vStream, fStream;
		vStream << vFile.rdbuf();
		fStream << fFile.rdbuf();
		vFile.close();
		fFile.close();
		vertexCode = vStream.str();
		fragmentCode = fStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char *vShaderCode = vertexCode.c_str();
	const char *fShaderCode = fragmentCode.c_str();
	unsigned int vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


Shader::~Shader()
{
	
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string & name, bool value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setBool2(const std::string & name, bool value, bool value2) const
{
	glUniform2i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2);
}

void Shader::setBool3(const std::string & name, bool value, bool value2, bool value3) const
{
	glUniform3i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2, (int)value3);
}

void Shader::setBool4(const std::string & name, bool value, bool value2, bool value3, bool value4) const
{
	glUniform4i(glGetUniformLocation(ID, name.c_str()), (int)value, (int)value2, (int)value3, (int)value4);
}

void Shader::setInt(const std::string & name, int value)const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt2(const std::string & name, int value, int value2) const
{
	glUniform2i(glGetUniformLocation(ID, name.c_str()), value, value2);
}

void Shader::setInt3(const std::string & name, int value, int value2, int value3) const
{
	glUniform3i(glGetUniformLocation(ID, name.c_str()), value, value2, value3);
}

void Shader::setInt4(const std::string & name, int value, int value2, int value3, int value4) const
{
	glUniform4i(glGetUniformLocation(ID, name.c_str()), value, value2, value3, value4);
}

void Shader::setFloat(const std::string & name, float value)const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat2(const std::string & name, float value, float value2) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value, value2);
}

void Shader::setFloat3(const std::string & name, float value, float value2, float value3) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value, value2, value3);
}

void Shader::setFloat4(const std::string & name, float value, float value2, float value3, float value4) const
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value, value2, value3, value4);
}

void Shader::setMat4(const std::string & name, glm::mat4 matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

