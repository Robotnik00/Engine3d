#version 300

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec3 tex;

layout (location = 0) out vec3 v;
layout (location = 1) out vec3 N;
layout (location = 2) out vec3 texcoords;

void main()
{
	N = normals;
	v = VertexPosition;
	texcoords = tex;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition,1.0);
}
