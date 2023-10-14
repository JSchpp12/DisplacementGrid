#version 450 

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal; 
layout(location = 2) in vec3 inColor;		//vertex color
layout(location = 3) in vec2 inTexCoord;	//texture coordinate for vertex 
layout(location = 4) in vec3 inMatAmbient; 
layout(location = 5) in vec3 inMatDiffuse; 
layout(location = 6) in vec3 inMatSpecular; 
layout(location = 7) in float inMatShininess; 

layout(binding = 0, set = 0) uniform GlobalUniformBufferObject {
	mat4 proj;
	mat4 view;  
	mat4 inverseView; 
	int numLights; 
} globalUbo; 

layout(binding = 0, set = 1) uniform uniformBufferObject{
	mat4 modelMatrix; 
	mat4 normalModelMatrix; 
} objectUbo;

layout(location = 0) out vec3 fragColor; 
layout(location = 1) out vec2 fragTextureCoordinate; 
layout(location = 2) out vec3 fragPositionWorld;	//fragment's position in world space
layout(location = 3) out vec3 fragNormalWorld;		//fragment's normal in world space 

void main() {
	vec4 positionWorld = objectUbo.modelMatrix * vec4(inPosition, 1.0); 

	//will eventually do displacement in direction of normal here

	fragColor = inColor; 
	fragTextureCoordinate = vec2(0.0); 		//unused -- complicated in vulkan to remove vert attribute (for speed keep for now)
	fragPositionWorld = positionWorld.xyz;
	fragNormalWorld = inNormal; 

	gl_Position = globalUbo.proj * globalUbo.view * positionWorld; 
}