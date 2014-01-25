// taken from http://www.opengl.org/sdk/docs/tutorials/ClockworkCoders/index.php

#version 300

layout (location = 0) in vec3 N;
layout (location = 1) in vec3 v;    
layout (location = 2) in vec3 texcoords;

layout (location = 0) out vec4 FragColor;

uniform sampler2D Tex1;

void main (void)  
{  
   //vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
   //vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
   //vec3 R = normalize(-reflect(L,N));  
 
   //calculate Ambient Term:  
   //vec4 Iamb = vec4(0,0,0,1);//gl_FrontLightProduct[0].ambient;    

   //calculate Diffuse Term:  
   //vec4 Idiff = (.5) * max(dot(N,L), 0.0); // gl_FrontLightProduct[0].diffuse
   //Idiff = clamp(Idiff, 0.0, 1.0);     
   
   // calculate Specular Term:
   //vec4 Ispec = .01
   //             * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
   //Ispec = clamp(Ispec, 0.0, 1.0); //gl_FrontLightProduct[0].specular 
 
   //vec4 color = vec4(0,0,0,1);
	
   //gl_FragColor = vec4(.2,.1,.2,.5) + Iamb + Idiff + Ispec; //gl_FrontLightModelProduct.sceneColor

   //gl_FragColor = vec4(texcoords.x, texcoords.y, texcoords.z, 1);

   vec4 texcolor = texture(Tex1,texcoords.xy);
	
   FragColor = texcolor;
}

