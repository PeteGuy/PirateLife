#version 330 core

layout  (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;
//layout (location = 1) in vec3 normals;
//layout  (location = 2) in vec2 texCoord;
//layout (location = 3) in vec4 colors;

//out vec2 TexCoord;
out vec3 normalSurf;

out vec3 fragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMatrix;
//uniform vec3 camPosition;
uniform float timeValue;


void main()
{
	vec4 seaPos = model * vec4(position, 1.0);
	//seaPos.z = cos(timeValue+seaPos.x+seaPos.y);

	float height = 0.25 * cos(timeValue + seaPos.x/20+seaPos.y/20);
	fragPos = seaPos.xyz;
	seaPos = proj * view * vec4(seaPos.x,seaPos.y,height,1.0);//model * vec4(position.x,position.y+height ,position.z , 1.0);
	normalSurf  = normalMatrix * normals;
	gl_Position = seaPos;

	//TexCoord = vec2(texCoord.x, texCoord.y);
}
