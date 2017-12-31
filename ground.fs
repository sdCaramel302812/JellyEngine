#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 ourTex;
in vec3 Normal;
in vec3 FragPos;
//uniform sampler2D texture;
//uniform vec3 lightColor;
//uniform vec3 objectColor;
//uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material{
	vec3 specular;
	sampler2D diffuse;
	float shininess;
};

uniform Material material;

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};

struct ParallelLight {
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;
	vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;

	//切光角cos 值
	float cutoff;
	float outerCutoff;
};

#define NR_POINT_LIGHTS 4

uniform ParallelLight paraLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;

vec3 CalcParaLight(ParallelLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	vec3 result = CalcParaLight(paraLight, norm, viewDir);

	for(int i = 0; i < NR_POINT_LIGHTS; i++){
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	vec3 distance = viewPos - FragPos;
	float dist = sqrt(distance.x * distance.x + distance.y * distance.y + distance.z * distance.z);
	float fogFactor = (50 - dist) / (50.0 - 5);
	fogFactor = clamp( fogFactor, 0.0, 1.0 );
	vec3 fogColor = vec3(0.5, 0.5, 0.5);
	result *= fogFactor * fogColor;
	result = mix(fogColor, result, fogFactor);

	FragColor =vec4(result, 1.0);

}
// 

vec3 CalcParaLight(ParallelLight light, vec3 normal, vec3 viewDir){
	//漫反射
	vec3 dir = normalize(-light.direction);
    float diff = max(dot(normal, dir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.diffuse,ourTex))) * light.diffuse;

	//鏡面光
	vec3 reflectDir = reflect(-dir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec *  material.specular) * light.specular;

	//環境光
	vec3 ambient =light.ambient * vec3(texture(material.diffuse,ourTex));
	
	return (specular + ambient + diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	//漫反射
    vec3 dir = normalize(light.position - fragPos);
    float diff = max(dot(normal, dir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.diffuse,ourTex))) * light.diffuse;

	//鏡面光
	vec3 reflectDir = reflect(-dir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec *  material.specular) * light.specular;

	//環境光
	vec3 ambient =light.ambient * vec3(texture(material.diffuse,ourTex));

	//照度
	vec3 distance = light.position - fragPos;
	float dist = sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2));
	float candela = 5;
	float illumination = candela /(light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

	
	return illumination * (specular + ambient + diffuse);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	//手電筒效果
	//片段指向光源向量與光源方向向量夾角cos 值
	vec3 dir = normalize(light.position - fragPos);
	float theta = dot(dir, normalize(-light.direction));
	float epsilon   = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);   

	//漫反射
    float diff = max(dot(normal, dir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.diffuse,ourTex))) * light.diffuse * intensity;

	//鏡面光
	vec3 reflectDir = reflect(-dir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec *  material.specular) * light.specular * intensity;

	//環境光
	vec3 ambient =light.ambient * vec3(texture(material.diffuse,ourTex));

	//照度
	vec3 distance = light.position - FragPos;
	float dist = sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2));
	float candela = 20;
	float illumination = candela /(light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

	
	return  (specular + ambient + diffuse);
}


/*
	//手電筒效果
	//片段指向光源向量與光源方向向量夾角cos 值
	vec3 dir = normalize(light.position - FragPos);
	float theta = dot(dir, normalize(-light.direction));
	float epsilon   = light.cutoff - light.outerCutoff;
	float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);   

	//漫反射
	vec3 norm = normalize(Normal);
    //vec3 dir = normalize(lightPos - FragPos);//點光源
	//vec3 dir = normalize(-light.direction);//平行光
    float diff = max(dot(norm, dir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.diffuse,ourTex))) * light.diffuse * intensity;

	//鏡面光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-dir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * material.specular) * light.specular * intensity;

	//環境光
	vec3 ambient =light.ambient * vec3(texture(material.diffuse,ourTex));

	//照度
	//vec3 distance = lightPos - FragPos;
	vec3 distance = light.position - FragPos;
	float dist = sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2));
	float candela = 20;
	float illumination = candela /(light.constant + light.linear * dist + light.quadratic * pow(dist, 2));

	
	vec3 result =  illumination * (specular + ambient + diffuse);
	FragColor =vec4(result, 1.0);
*/