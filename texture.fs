#version 330 core
out vec4 FragColor;
in vec2 ourTex;

struct Material{

	sampler2D diffuse;

};

uniform Material material;


void main()
{

	vec3 result =vec3(texture(material.diffuse,ourTex));


	FragColor =vec4(result, 0.8);
}
// 
