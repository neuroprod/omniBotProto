#version 150


in vec3 vNormal;


out vec4 outColor;
in float r;


void main()
{
    
  
    outColor = vec4( r *vec3(1.0,0.0,0.0), 1.0);
  
}
