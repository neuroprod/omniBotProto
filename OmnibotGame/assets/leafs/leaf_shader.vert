#version 150

uniform mat4	ciModelViewProjection;
uniform mat4 ciModelMatrix;

in vec4		ciPosition;
in vec3		ciNormal;
in vec2 vInstanceColor;
in mat4	vInstancePosition;


out vec2	vTexCoord0;


out vec3	normal;





void main( void )
{
    
 
    gl_Position	= ciModelViewProjection *(vInstancePosition*ciPosition);
    
    vTexCoord0 = vInstanceColor;
	normal = mat3(ciModelMatrix*vInstancePosition)*ciNormal;
    
}
