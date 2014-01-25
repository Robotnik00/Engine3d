// taken from http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/index.php

/*#version 400

in vec3 vertexcoords;
in vec3 normals;
in vec3 texcoors;




out vec3 v;

out vec3 N;

out vec3 vTexCoord;

out vec4 raw;


void main(void)
{

   v = vec3(gl_ModelViewMatrix * gl_Vertex);       
   N = normalize(gl_NormalMatrix * gl_Normal);
   vTexCoord = gl_MultiTexCoord0;
	
   vec4 data = vec4(vertexcoords, 1);

   raw = data;
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

}*/
          

#version 300
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec3 tex;

layout (location = 0) out vec3 N;
layout (location = 1) out vec3 v;
layout (location = 2) out vec3 texcoords;

void main()
{
	N = normals;
	v = VertexPosition;
	texcoords = tex;

	gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition,1.0);
}
