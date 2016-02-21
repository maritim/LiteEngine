#version 120

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 modelViewProjectionMatrix;
uniform mat3 normalMatrix;

varying vec3 position;
varying vec3 normal;
varying vec3 lightpos;

void main()
{
	gl_Position = modelViewProjectionMatrix * gl_Vertex;

	position = vec3 (modelViewMatrix * gl_Vertex);
	normal = normalMatrix * gl_Normal;
	lightpos = vec3 (modelViewMatrix * gl_LightSource [0].position);
}