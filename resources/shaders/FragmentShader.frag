#version 130

in vec3 vertex;    
in vec3 normal;
in vec3 texcoords;

out vec4 FragColor;

struct LightParameters
{
  vec3 position;
  vec3 diffuse;
  vec3 specular;
  float constantAttenuation, linearAttenuation, quadraticAttenuation;
  float spotCutoff, spotExponent;
  vec3 spotDirection, ambientLight;
};


struct MaterialParameters 
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};



uniform sampler2D Tex1;
uniform MaterialParameters material;
uniform LightParameters light1;

uniform mat4 cameraposition = mat4(1);

void main (void)  
{  
  float attenuation = 1;
 
  vec3 lightdir = normalize(light1.position - vertex);

  vec3 distVec = light1.position - vertex;
  float distance = length(distVec);
  
  attenuation = 1/(light1.constantAttenuation 
                + light1.linearAttenuation * distance 
                + light1.quadraticAttenuation*distance*distance);

  if(light1.spotCutoff <= 90.0)
  {

    float clampedCos = max(0, dot(-lightdir, normalize(light1.spotDirection)));
    if(clampedCos < cos(radians(light1.spotCutoff)))
    {
      attenuation = 0;
    }
    else 
    {
      attenuation = attenuation * pow(clampedCos, light1.spotExponent);
    }
  }

  vec4 Iamb = vec4(light1.ambientLight * material.ambient, 0);
  vec4 Idiff = vec4(attenuation * material.diffuse * light1.diffuse * max(0, dot(normal,lightdir)),1);
 
  vec4 Ispec;
  if(dot(normal,lightdir) < 0)
  {
    Ispec = vec4(0);
  }
  else 
  {
    Ispec = vec4(attenuation * light1.specular * material.specular
            * pow(max(0,dot(reflect(-lightdir, normal), normalize(vertex))),material.shininess),0);
  }
  

  vec4 texcolor = vec4(1,1,1,1);
  if(texcoords.x != 0 && texcoords.y != 0)
  {
      texcolor = texture(Tex1, texcoords.xy);
      if(texcolor.x == 0 && texcolor.y == 0 && texcolor.z == 0)
      {
          texcolor = vec4(1,1,1,1);
      }
  }


  //vec4 texcolor = vec4(1,0,0,1);

  FragColor = texcolor * (Idiff +  Iamb + Ispec);
}

