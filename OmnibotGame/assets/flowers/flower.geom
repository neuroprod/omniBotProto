#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 27) out;
uniform float uTime;
uniform mat4 ciModelMatrix;
uniform mat4 ciViewMatrix;
uniform mat4 ciProjectionMatrix;
in vec3 dir_in[];
in vec3 flat_in[];
out vec3 vNormal;
out vec2 vUV;


void drawTriangle(vec4 pos1,vec2 uv1,vec4 pos2,vec2 uv2,vec4 pos3,vec2 uv3)
{ 

    vNormal = normalize(cross( pos2.xyz-pos1.xyz, pos3.xyz-pos1.xyz));
    
    gl_Position = ciProjectionMatrix* ciViewMatrix *  ciModelMatrix * pos1;
   vUV =uv1;
    EmitVertex();
    
    gl_Position = ciProjectionMatrix * ciViewMatrix *  ciModelMatrix    *pos2;
    vUV =uv2;
    EmitVertex();

    gl_Position = ciProjectionMatrix * ciViewMatrix * ciModelMatrix * pos3;
    vUV =uv3;
    EmitVertex();
    EndPrimitive();

}
void drawTriangleDouble(vec4 pos1,vec2 uv1,vec4 pos2,vec2 uv2,vec4 pos3,vec2 uv3)
{
	drawTriangle( pos1,uv1, pos2,uv2,pos3,uv3);
	drawTriangle( pos2,uv2,pos1,uv1,pos3,uv3);
}

mat4 getRotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    

    vec4 pos = gl_in[0].gl_Position*2.0;
	pos.w =1;
	  vec3 dir = dir_in[0];
  
  

    vec3 flatl =  flat_in[0];
	mat4 rotationMatrix  = getRotationMatrix(vec3(flatl.xy,0.0),flatl.z*-1.2 );

	float base =15.0;
	vec2 dir1 = normalize(dir.xy)*base;
	vec2 dir2 = dir1.yx; 
	dir2.y*=-1;
	////////////////////////////
    vec4 pos0  ;
	pos0.xy =-dir1 -dir2;
	pos0.z=dir.z;
	pos0.w=1.0;
	  pos0= pos+pos0;

    vec4 pos1  ;
	pos1.xy =dir1 -dir2;
	pos1.w=1.0;
	pos1.z=dir.z;
    pos1= pos+pos1;

	////////////////////////////
	 vec4 pos2 ;
	pos2.xy = dir1 +dir2;
	pos2.z=dir.z;
	pos2.w=1.0;
	pos2= pos+ rotationMatrix*pos2;


    vec4 pos3 ;
	pos3.xy = -dir1 +dir2;
	pos3.z =dir.z;
	pos3.w=1.0;
	pos3= pos+ rotationMatrix*pos3;

	vec2 uv0 =vec2(0,0);
	vec2 uv1 =vec2(1,0);
	vec2 uv2 =vec2(1,1);
	vec2 uv3 =vec2(0,1);


    drawTriangleDouble(pos0,uv0,pos2,uv2 ,pos1,uv1);
	drawTriangleDouble(pos0,uv0,pos3,uv3 ,pos2,uv2);
	
    
    
    
    	
	
    
    
}
