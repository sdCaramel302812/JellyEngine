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
#include "AABBTree.h"
#include "UI.h"
#include "Character.h"
#include <map>
#include <ft2build.h>
#include "TextItem.h"
#include FT_FREETYPE_H 
#include <windows.h>
#include <unordered_map>

#define SCENE_WIDTH 1280
#define SCENE_HEIGHT 720

using std::cout;
using std::endl;


extern Camera camera;



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
		glBindBuffer(GL_ARRAY_BUFFER, VBOs.at(vbo));
		if (ebo != "") {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs.at(ebo));
		}
		int total_size = 0;
		for (int i = 0; i < length; ++i) {
			total_size += size[i];
		}

		for (int i = 0; i < length; ++i) {
			glVertexAttribPointer(pos[i], size[i], GL_FLOAT, GL_FALSE, total_size * sizeof(float), (void *)(shift[i] * sizeof(float)));
			glEnableVertexAttribArray(pos[i]);

		}

		//instance buffer
		glBindBuffer(GL_ARRAY_BUFFER, VBOs.at("instance"));
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 5 * vec4Size, (void*)0);
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 5 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 5 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(7);
		glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 5 * vec4Size, (void*)(3 * vec4Size));
		glEnableVertexAttribArray(8);
		glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 5 * vec4Size, (void*)(4 * vec4Size));

		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glVertexAttribDivisor(7, 1);
		glVertexAttribDivisor(8, 1);

		glBindVertexArray(0);
		//dic<unsigned int> data(key, VAO);
		VAOs.insert(std::make_pair(key, VAO));
	}

	static void addVAO(string key, int id) {
		//dic<unsigned int> data(key, id);
		VAOs.insert(std::make_pair(key, id));
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addVBO(float vertices[], int length, string key) {
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * length, vertices, GL_STATIC_DRAW);
		//dic<unsigned int> data(key, VBO);
		VBOs.insert(std::make_pair(key, VBO));
	}

	static void addVBO(string key, unsigned int id) {
		//dic<unsigned int> data(key, id);
		VBOs.insert(std::make_pair(key, id));
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addEBO(unsigned int indices[], int length, string key) {
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * length, indices, GL_STATIC_DRAW);
		//dic<unsigned int> data(key, EBO);
		EBOs.insert(std::make_pair(key, EBO));
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addTexture(string path, string key, GLint re = GL_REPEAT, GLint lin = GL_LINEAR) {
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, re);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, re);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, crChannel;
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(path.c_str(), &width, &height, &crChannel, STBI_rgb_alpha);
		if (data)
		{
			if (crChannel == 3) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			else if (crChannel == 4) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			}
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		//dic<unsigned int> Ddata(key, texture);
		textures.insert(std::make_pair(key, texture));
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void removeTexture(string key) {
		unsigned int texture = textures.at(key);
		glDeleteTextures(1,&texture);
		textures.erase(key);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addShader(string key, const GLchar *vpath, const GLchar *fpath) {
		Shader shader(vpath, fpath);
		//dic<Shader> data(key, shader);
		shaders.insert(std::make_pair(key, shader));
	}
	static void addShader(string key, Shader shader) {
		//dic<Shader> data(key, shader);
		shaders.insert(std::make_pair(key, shader));
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void draw(Entity *obj, GLenum type = GL_TRIANGLES) {
		//為了增加效能，將 use program 和 projection , view 移到函式外面
		
		glBindVertexArray(VAOs.at(obj->VAO));
		if (obj->texture != "") {
			glBindTexture(GL_TEXTURE_2D, Render::textures.at(obj->texture));
		}
		if (obj->specular != "") {
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, Render::textures.at(obj->specular));
		}

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(obj->rigid.data.position.x, obj->rigid.data.position.y, obj->rigid.data.position.z)) * obj->_model_matrix;
		Render::shaders.at(obj->shader).setMat4("model", model);
		
		if (obj->EBO == "") {
			glDrawArrays(type, 0, obj->size());
		}
		else {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Render::EBOs.at(obj->EBO));
			glDrawElements(type, obj->size(), GL_UNSIGNED_INT, 0);
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////
	static void draw(UI *ui) {
		glm::mat4 model;
		glm::mat4 scale = glm::scale(model, glm::vec3(ui->width(), ui->height(), 1));
		glm::mat4 translate = glm::translate(model, glm::vec3(ui->x(), ui->y(), ui->getZ()));
		model = translate*scale;


		glBindVertexArray(VAOs.at(ui->_vao));
		if (ui->_texture != "") {
			shaders.at("texture").setMat4("model", model);
			glBindTexture(GL_TEXTURE_2D, Render::textures.at(ui->_texture));
		}
		else {
			shaders.at("color").setFloat4("color", ui->_color.x, ui->_color.y, ui->_color.z, ui->_color.w);
			shaders.at("color").setMat4("model", model);
		}

		glDrawArrays(ui->_draw_type, 0, 6);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void draw(UI *ui, int editor) {			//editor 僅做為指示用途，傳入任意數字即可，此 function 會依照 camera 投影
		glm::mat4 model;
		glm::mat4 scale = glm::scale(model, glm::vec3(ui->width(), ui->height(), 1));
		glm::mat4 translate = glm::translate(model, glm::vec3(ui->x(), ui->y(), ui->getZ()));
		model = translate*scale;


		glBindVertexArray(VAOs.at(ui->_vao));
		if (ui->_texture != "") {
			//Render::shaders.search("texture").use();
			Render::shaders.at("texture").setMat4("model", model);
			glBindTexture(GL_TEXTURE_2D, Render::textures.at(ui->_texture));
		}
		else {
			shaders.at("color").setFloat4("color", ui->_color.x, ui->_color.y, ui->_color.z, ui->_color.w);
			shaders.at("color").setMat4("model", model);
		}


		glDrawArrays(ui->_draw_type, 0, 6);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void updateInstance(glm::mat4 matrix[], int length) {
		unsigned int VBO = VBOs.at("instance");
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * length, matrix, GL_STREAM_DRAW);
	}
	static void updateInstance(glm::vec4 matrix[], int length) {
		unsigned int VBO = VBOs.at("instance");
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * length * 5, matrix, GL_STREAM_DRAW);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void drawAABB(AABB _aabb) {
		float aabb[54] = {
			_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
			_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
			_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._left_down_back.z,
			_aabb._left_down_back.x,_aabb._right_top_front.y, _aabb._left_down_back.z,
			_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
			_aabb._left_down_back.x,_aabb._right_top_front.y, _aabb._left_down_back.z,
			_aabb._left_down_back.x, _aabb._right_top_front.y, _aabb._right_top_front.z,
			_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
			_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
			_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
			_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._left_down_back.z,
			_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._right_top_front.z,
			_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
			_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._left_down_back.z,
			_aabb._right_top_front.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
			_aabb._left_down_back.x, _aabb._left_down_back.y, _aabb._right_top_front.z,
			_aabb._left_down_back.x, _aabb._right_top_front.y, _aabb._right_top_front.z,
			_aabb._right_top_front.x, _aabb._right_top_front.y, _aabb._right_top_front.z,		
		};
		unsigned int VBO = VBOs.at("debuger");
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(aabb), aabb, GL_STATIC_DRAW);
		unsigned int VAO = VAOs.at("debuger");
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE_STRIP, 0, 18);
		///////////////////////////
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void drawLine(glm::vec3 pos, float length, float radius, glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)) {	//角度
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1920), 0.0f, static_cast<GLfloat>(1080), 0.0f, 10.0f);
		glm::mat4 view;
		glm::mat4 scale;
		scale = glm::scale(scale, glm::vec3(length, 1, 1));
		glm::mat4 rotate;
		rotate = glm::rotate(rotate, glm::radians(radius), glm::vec3(0, 0, 1));
		glm::mat4 translate;
		translate = glm::translate(translate, pos);
		glm::mat4 model = translate*rotate*scale;
		glBindVertexArray(VAOs.at("line_vao"));
		shaders.at("color").use();
		shaders.at("color").setMat4("projection", projection);
		shaders.at("color").setMat4("view", view);
		shaders.at("color").setFloat4("color", color.x, color.y, color.z, color.w);
		shaders.at("color").setMat4("model", model);
		glDrawArrays(GL_LINES, 0, 2);
	}
	static void drawLine(glm::vec3 pos, float length, float radius, glm::vec4 color, int editor) {	//弧度
		glm::mat4 scale;
		scale = glm::scale(scale, glm::vec3(length, 1, 1));
		glm::mat4 rotate;
		rotate = glm::rotate(rotate, radius, glm::vec3(0, 1, 0));
		rotate = glm::rotate(rotate, glm::radians(-90.0f), glm::vec3(0, 1, 0));
		glm::mat4 translate;
		translate = glm::translate(translate, pos);
		glm::mat4 model = translate*rotate*scale;
		glBindVertexArray(VAOs.at("line_vao"));
		shaders.at("color").setFloat4("color", color.x, color.y, color.z, color.w);
		shaders.at("color").setMat4("model", model);
		glDrawArrays(GL_LINES, 0, 2);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void addGlyph(wchar_t c) {
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		}

		FT_Face face;
		if (FT_New_Face(ft, "../fonts/SourceHanSansTC-Normal.otf", 0, &face)) {
			std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		}

		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Load first 128 characters of ASCII set
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<wchar_t, Character>(c, character));

		glBindTexture(GL_TEXTURE_2D, 0);
		// Destroy FreeType once we're finished
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void draw(TextItem *text_item) {
		float z = text_item->getZ();
		glm::vec3 color = glm::vec3(1, 1, 1);
		std::wstring text = text_item->text.data();
		float scale = text_item->scale;
		float line_width = text_item->line_width;
		float x = text_item->x();
		float y = text_item->y();
		shaders.at("text").setFloat3("textColor", color.x, color.y, color.z);
		glBindVertexArray(VAOs.at("text"));
		

		float left_x = x;

		std::wstring::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			if (Render::Characters.find(*c) == Render::Characters.end()) {
				Render::addGlyph(*c);
			}
			Character ch = Render::Characters.at(*c);

			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// 对每个字符更新VBO
			GLfloat vertices[6][5] = {
				{ xpos,     ypos + h,   0.0, 0.0 , z },
				{ xpos,     ypos,       0.0, 1.0 , z },
				{ xpos + w, ypos,       1.0, 1.0 , z },

				{ xpos,     ypos + h,   0.0, 0.0 , z },
				{ xpos + w, ypos,       1.0, 1.0 , z },
				{ xpos + w, ypos + h,   1.0, 0.0 , z }
			};
			// 在四边形上绘制字形纹理
			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			// 更新VBO内存的内容
			glBindBuffer(GL_ARRAY_BUFFER, VBOs.at("text"));
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// 绘制四边形
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// 更新位置到下一个字形的原点，注意单位是1/64像素
			x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)

											//若超過限制寬度則換行
			if (line_width != 0 && x - left_x > line_width) {
				x = left_x;
				y -= (ch.Advance >> 6)*scale;
			}
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void drawText(wstring text, float x, float y, float scale, glm::vec3 color, float line_width = 0, float z = 1) {
		z *= -1;
		shaders.at("text").use();
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1920), 0.0f, static_cast<GLfloat>(1080), 0.0f, 10.0f);
		shaders.at("text").setFloat3("textColor", color.x, color.y, color.z);
		shaders.at("text").setMat4("projection", projection);
		glBindVertexArray(VAOs.at("text"));

		float left_x = x;

		std::wstring::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			if (Render::Characters.find(*c) == Render::Characters.end()) {
				Render::addGlyph(*c);
			}
			Character ch = Render::Characters.at(*c);

			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// 对每个字符更新VBO
			GLfloat vertices[6][5] = {
				{ xpos,     ypos + h,   0.0, 0.0 , z },
				{ xpos,     ypos,       0.0, 1.0 , z },
				{ xpos + w, ypos,       1.0, 1.0 , z },

				{ xpos,     ypos + h,   0.0, 0.0 , z },
				{ xpos + w, ypos,       1.0, 1.0 , z },
				{ xpos + w, ypos + h,   1.0, 0.0 , z }
			};
			// 在四边形上绘制字形纹理
			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			// 更新VBO内存的内容
			glBindBuffer(GL_ARRAY_BUFFER, VBOs.at("text"));
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// 绘制四边形
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// 更新位置到下一个字形的原点，注意单位是1/64像素
			x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)

			//若超過限制寬度則換行
			if (line_width != 0 && x - left_x > line_width) {
				x = left_x;
				y -= (ch.Advance >> 6)*scale;
			}
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	/////////////////////////////////////////////////////////////////////////////////
	static void drawText(TString txt, float x, float y, float scale, glm::vec3 color, float line_width = 0, float z = 1) {
		z*=-1;
		shaders.at("text").use();
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(1920), 0.0f, static_cast<GLfloat>(1080), 0.0f, 10.0f);
		shaders.at("text").setFloat3("textColor", color.x, color.y, color.z);
		shaders.at("text").setMat4("projection", projection);
		glBindVertexArray(VAOs.at("text"));

		wstring text = txt.data();
		float left_x = x;

		std::wstring::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			if (Render::Characters.find(*c) == Render::Characters.end()) {
				Render::addGlyph(*c);
			}
			Character ch = Render::Characters.at(*c);

			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// 对每个字符更新VBO
			GLfloat vertices[6][5] = {
				{ xpos,     ypos + h,   0.0, 0.0 , z},
				{ xpos,     ypos,       0.0, 1.0 , z},
				{ xpos + w, ypos,       1.0, 1.0 , z},

				{ xpos,     ypos + h,   0.0, 0.0 , z},
				{ xpos + w, ypos,       1.0, 1.0 , z},
				{ xpos + w, ypos + h,   1.0, 0.0 , z}
			};
			// 在四边形上绘制字形纹理
			glBindTexture(GL_TEXTURE_2D, ch.textureID);
			// 更新VBO内存的内容
			glBindBuffer(GL_ARRAY_BUFFER, VBOs.at("text"));
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// 绘制四边形
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// 更新位置到下一个字形的原点，注意单位是1/64像素
			x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)

			if (line_width != 0 && x - left_x > line_width) {
				x = left_x;
				y -= (ch.Advance >> 6)*scale;
			}
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	/////////////////////////////////////////////////////////////////////////////////
	
	static std::unordered_map<string, unsigned int> VAOs;
	static std::unordered_map<string, unsigned int> VBOs;
	static std::unordered_map<string, unsigned int> EBOs;
	static std::unordered_map<string, unsigned int> textures;
	static std::unordered_map<string, Shader> shaders;
	static std::map<wchar_t, Character> Characters;
};


