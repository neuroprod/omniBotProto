#version 150
uniform mat4 ciModelMatrix;


in vec4 ciPosition;
in vec4 ciColor;





out vec4 color;



void main()
{
   
    gl_Position = ciModelMatrix * ciPosition;
   color=ciColor;
    
}
