#version 150

uniform mat4	ciModelViewProjection;
uniform mat4 ciModelMatrix;
uniform mat4 uShadowMatrix;
in vec4		ciPosition;
in vec2 vInstanceColor;

in mat4	vInstancePosition;


out vec2	vTexCoord0;
out vec4 vShadowCoord;


const mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.5, 0.5, 0.5, 1.0 );




void main( void )
{
    
   // gl_Position	= ciModelViewProjection *(vec4(vInstancePosition[3][0],vInstancePosition[3][1],vInstancePosition[3][2],0)+ciPosition);
    gl_Position	= ciModelViewProjection *(vInstancePosition*ciPosition);
    
    vTexCoord0 = vInstanceColor;
    vShadowCoord	= ( biasMatrix * uShadowMatrix * ciModelMatrix ) * vInstancePosition*ciPosition;
    
}
