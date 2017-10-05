#version 150
uniform mat4 ciModelMatrix;


in vec4 ciPosition;
in vec3 ciColor;
in vec3 ciTexCoord0;
in float ciTexCoord1;



out vec3 text;
out vec3 color;
out float flatD;
out vec4 posN;


void main()
{
    posN =ciPosition;
    gl_Position = ciModelMatrix * ciPosition;
   color=ciColor;
    
   // vvShadowCoord	= ( biasMatrix * uShadowMatrix * ciModelMatrix ) * ciPosition;
   flatD =ciTexCoord1;
    text =ciTexCoord0;
}
