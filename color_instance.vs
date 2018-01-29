#version 400 core
layout(location = 0) in vec3 aPos;
layout(location = 5) in mat4 instanceMatrix;	//4 vec4, location : 5, 6, 7, 8

uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * instanceMatrix *vec4(aPos, 1.0);
}// 