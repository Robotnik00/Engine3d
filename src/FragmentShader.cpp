#version 300

layout (location = 0) in vec3 N;
layout (location = 1) in vec3 v;    
layout (location = 2) in vec3 texcoords;

layout (location = 0) out vec4 FragColor;

uniform sampler2D Tex1;

void main (void)  
{  

   vec4 texcolor = texture(Tex1,texcoords.xy);
	
   FragColor = texcolor;
}

