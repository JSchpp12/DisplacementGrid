#version 450 

layout(location = 0) in vec3 inFragColor;
layout(location = 1) in vec2 inFragTextureCoordinate; 
layout(location = 2) in vec3 inFragPositionWorld;
layout(location = 3) in vec3 inFragNormalWorld;

layout(location = 0) out vec4 outColor;

struct Light{
	vec4 position;

	//properties
	vec4 ambient; 
	vec4 diffuse;
	vec4 specular; 
};

layout(binding = 0, set = 0) uniform GlobalUniformBufferObject {
	mat4 proj;
	mat4 view;  
	mat4 inverseView; 
	uint numLights; 
uint renderSettings;
} globalUbo; 

 layout(binding = 1, set = 0) buffer globalLightBuffer{
	Light lights[];
 };

void main() {
	outColor = vec4(1.0, 1.0, 1.0, 1.0); 
}