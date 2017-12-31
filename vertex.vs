#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 TexCood;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 ourTex;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal =  vec3(model * vec4(aNormal, 0.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourTex = TexCood;
}

//not recommend to use inverse
//mat3(transpose(inverse(model))) * aNormal;