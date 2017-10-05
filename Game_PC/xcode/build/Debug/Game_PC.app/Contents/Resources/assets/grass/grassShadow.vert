#version 150

in vec4 ciPosition;
in vec3 ciColor;
in vec3 ciTexCoord0;
in float ciTexCoord1;
uniform mat4 ciModelMatrix;

out vec3 text;
out vec3 color;







out float flatD;


void main()
{
    
    gl_Position = ciModelMatrix * ciPosition;
    color=ciColor;
   
    
    flatD =ciTexCoord1;
    text =ciTexCoord0;
}
