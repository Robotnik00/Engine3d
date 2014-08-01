#version 130

in vec3 VertexPosition;
in vec3 Normal;
in vec3 TexCoords;

out vec3 vertex;
out vec3 normal;
out vec3 texcoords;

void main()
{
  mat4 transform = gl_ModelViewMatrix;
  transform[3][0] = 0;
  transform[3][1] = 0;
  transform[3][2] = 0; 

  vertex = (gl_ModelViewMatrix * vec4(VertexPosition,1)).xyz;
  normal = normalize(transform * vec4(Normal,1)).xyz;
  texcoords = TexCoords;

  gl_Position = gl_ModelViewProjectionMatrix * vec4(VertexPosition,1.0);
}
