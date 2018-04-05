#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 foo;
//TexCood and modelMatrix are instance
layout(location = 4) in vec4 TexCood;		//x,y :position, z :width, w :height
layout(location = 5) in mat4 modelMatrix;	//4 vec4, location : 5, 6, 7, 8

uniform mat4 view;
uniform mat4 projection;

out vec2 ourTex;

void main()
{
	gl_Position = projection * view * modelMatrix * vec4(aPos, 1.0);
	ourTex = vec2(TexCood.x + aPos.z * TexCood.z, TexCood.y + aPos.x * TexCood.w);
}
