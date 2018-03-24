#version 150 core

uniform mat4 ciModelViewProjection;
uniform mat4 ciModelMatrix;
uniform mat3 ciNormalMatrix;
uniform mat4 ciModelView;
//uniform mat4 uShadowMatrix;
in vec2 ciTexCoord0;

in vec4	ciPosition;
out vec2	vTexCoord0;
//out vec4 vShadowCoord;

const mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
							  0.0, 0.5, 0.0, 0.0,
							  0.0, 0.0, 0.5, 0.0,
							  0.5, 0.5, 0.5, 1.0 );

void main( void )
{
    vTexCoord0 =ciTexCoord0;
	//vShadowCoord	= ( biasMatrix * uShadowMatrix * ciModelMatrix ) * ciPosition;
	gl_Position		= ciModelViewProjection * ciPosition;
}
