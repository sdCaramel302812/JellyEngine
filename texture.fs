#version 330 core
out vec4 FragColor;
in vec2 ourTex;

in vec4 texcood;

struct Material{

	sampler2D diffuse;

};

uniform Material material;


void main()
{

	vec4 result =vec4(texture(material.diffuse,ourTex));

	if(result.a<0.1){
		discard;
	}
	FragColor =result;
}
// 
