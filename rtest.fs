#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTex;
in vec3 Normal;
in vec3 FragPos;
//uniform sampler2D texture;

struct Material{

	sampler2D diffuse;

};

uniform Material material;


void main()
{

	vec3 result =vec3(texture(material.diffuse,ourTex));


	FragColor =vec4(result, 1.0);

}
// 
