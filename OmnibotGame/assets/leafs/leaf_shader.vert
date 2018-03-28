#version 150

uniform mat4	ciModelViewProjection;
uniform mat4 ciModelMatrix;
uniform mat4 uShadowMatrix;


in vec4		ciPosition;
in vec3		ciNormal;
in vec2 vInstanceColor;
in mat4	vInstancePosition;


out vec2	vTexCoord0;
out vec4 vShadowCoord;
out vec3	normal;




const mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
							  0.0, 0.5, 0.0, 0.0,
							  0.0, 0.0, 0.5, 0.0,
							  0.5, 0.5, 0.5, 1.0 );

void main( void )
{
    
	vShadowCoord	= ( biasMatrix * uShadowMatrix * ciModelMatrix*vInstancePosition ) * ciPosition;
 
    gl_Position	= ciModelViewProjection *vInstancePosition*ciPosition;
    
    vTexCoord0 = vInstanceColor;
	normal = mat3(ciModelMatrix*vInstancePosition)*ciNormal;
    
}
