#pragma once
#define GLEW_STATIC
#include <GL\glew.h>
// GLFW ( make you a windows that support opengl operation to work fine with your platform )
#include <GLFW\glfw3.h>
#include <iostream>
#include <vector>
#include "nstdlib.h"
#include "stb_image.h"
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"
using std::cout;
using std::endl;

using nstd::dic;
using nstd::hash;

extern TPCamera TPcamera;
extern Camera2D camera2d;

class Render
{
public:
	Render() {};
	~Render() {};
	/////////////////////////////////////////////////////////////////////////////////
	static void addVAO(int pos[], int size[], int shift[], int length, string key, string vbo, string ebo = "") {//需先加入 VBO 和 EBO 後再加入 VAO
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs.search(vbo));
		if (ebo != "") {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs.search(ebo));
		}
		int total_size = 0;
		for (int i = 0; i < length; ++i) {
			total_size += size[i];
		}

		for (int i = 0; i < length; ++i) {
			glVertexAttribPointer(pos[i], size[i], GL_FLOAT, GL_FALSE, total_size * sizeof(float), (void *)(shift[i] * sizeof(float)));
			glEnableVertexAttribArray(pos[i]);

		}
		glBindVertexArray(0);
		dic<unsigned int> data(key, VAO);
		VAOs.insert(data);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addVBO(float vertices[], int length, string key) {
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * length, vertices, GL_STATIC_DRAW);
		dic<unsigned int> data(key, VBO);
		VBOs.insert(data);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addEBO(unsigned int indices[], int length, string key) {
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * length, indices, GL_STATIC_DRAW);
		dic<unsigned int> data(key, EBO);
		EBOs.insert(data);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addTexture(string path, string key, GLint re = GL_REPEAT, GLint lin = GL_LINEAR) {
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, re);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, re);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, lin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, lin);

		int width, height, crChannel;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &crChannel, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		dic<unsigned int> Ddata(key, texture);
		textures.insert(Ddata);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addShader(string key, const GLchar *vpath, const GLchar *fpath) {
		Shader shader(vpath, fpath);
		dic<Shader> data(key, shader);
		shaders.insert(data);
	}
	static void addShader(string key, Shader shader) {
		dic<Shader> data(key, shader);
		shaders.insert(data);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void draw(Entity obj, GLenum type = GL_TRIANGLES) {
		(shaders.search(obj.shader)).use();
		glBindVertexArray(VAOs.search(obj.VAO));
		if (obj.texture != "") {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Render::textures.search(obj.texture));
		}
		if (obj.specular != "") {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, Render::textures.search(obj.specular));
		}
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
		//projection = glm::ortho(0, 1920, 0, 1080, 0, 100);
		//projection = glm::translate(projection, glm::vec3(0.0f, 0.0f, 0.0f));
		//projection = glm::scale(projection, glm::vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 view;
		view = TPcamera.getViewMatrix();/////////look out camera type
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(obj.rigid.data.position.x, obj.rigid.data.position.y, obj.rigid.data.position.z));
		Render::shaders.search(obj.shader).setMat4("view", view);
		Render::shaders.search(obj.shader).setMat4("projection", projection);
		Render::shaders.search(obj.shader).setMat4("model", model);
		if (obj.EBO == "") {
			glDrawArrays(type, 0, obj.size());
		}
		else {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::EBOs.search(obj.EBO));
			glDrawElements(type, obj.size(), GL_UNSIGNED_INT, 0);
		}
	}
	/////////////////////////////////////////////////////////////////////////////////
	static hash<unsigned int> VAOs;
	static hash<unsigned int> VBOs;
	static hash<unsigned int> EBOs;
	static hash<unsigned int> textures;
	static hash<Shader> shaders;
};


