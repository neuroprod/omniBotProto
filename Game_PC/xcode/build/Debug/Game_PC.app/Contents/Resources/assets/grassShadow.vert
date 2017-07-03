#version 150

in vec4 ciPosition;
in vec3 ciColor;
in vec3 ciTexCoord0;
in float ciTexCoord1;
uniform mat4 ciModelView;

out vec3 text;
out vec3 color;







out float flatD;


void main()
{
    
    gl_Position = ciModelView * ciPosition;
   color=ciColor;
    text =ciTexCoord0;
    
    flatD =ciTexCoord1;
}
