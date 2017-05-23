#version 150

uniform mat4	ciModelViewProjection;
uniform mat3	ciNormalMatrix;

in vec4		ciPosition;

in vec4		vInstancePosition; // per-instance position variable
in float	vInstanceColor;
in vec4		vInstanceAxisScale;


in vec2			ciTexCoord0;











mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}






void main( void )
{
    mat4 rMat =rotationMatrix(vInstanceAxisScale.xyz,vInstancePosition.w);
   
    gl_Position	= ciModelViewProjection *((rMat *ciPosition)+ vec4( vInstancePosition.xyz, 0 ) );
    
    
   
}
