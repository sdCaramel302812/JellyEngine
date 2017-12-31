#include "Render.h"

hash<unsigned int> Render::VAOs = hash<unsigned int>();
hash<unsigned int> Render::VBOs = hash<unsigned int>();
hash<unsigned int> Render::EBOs = hash<unsigned int>();
hash<unsigned int> Render::textures = hash<unsigned int>();
hash<Shader> Render::shaders = hash<Shader>();

/*
Render::Render()
{
		VAOs = new BufferHash(hashSize);
		VBOs = new BufferHash(hashSize);
		EBOs = new BufferHash(hashSize);
		textures = new BufferHash(hashSize);
		shaders = new ShaderHash(hashSize);
}

Render::~Render()
{
}

void Render::addVAO(vector<int> pos, vector<int> size, vector<int> shift, string key)
{
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		int total_size = 0;
		for (int i = 0; i < size.size(); ++i) {
			total_size += size.size();
		}
		for (int i = 0; i < size.size(); ++i) {
			glVertexAttribPointer(pos.at(i), size.at(i), GL_FLOAT, GL_FALSE, total_size * sizeof(float), (void *)shift.at(i));
			glEnableVertexAttribArray(pos.at(i));
		}
		dictionary data(key, VAO);
		VAOs->insert(data);
}

void Render::addVBO(float vertices[], string key)
{
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		dictionary data(key, VBO);
		VBOs->insert(data);
}

void Render::addEBO(float indices[], string key)
{
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		dictionary data(key, EBO);
		EBOs->insert(data);
}

void Render::addTexture(string path, string key, GLint re, GLint lin)
{
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
		dictionary Ddata(key, texture);
		textures->insert(Ddata);
}

void Render::addShader(string key, string vpath, string fpath)
{
		Shader shader("./vertex.vs", "./fragment.fs");
		shaderDictionary data(key, shader);
		shaders->insert(data);
}
*/