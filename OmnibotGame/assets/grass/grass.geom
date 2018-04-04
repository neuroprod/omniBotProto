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



void drawTriangle(vec4 pos1,vec4 pos2,vec4 pos3)
{ 

    vNormal = normalize(cross( pos2.xyz-pos1.xyz, pos3.xyz-pos1.xyz));
    
    gl_Position = ciProjectionMatrix* ciViewMatrix *  ciModelMatrix * pos1;
   
    EmitVertex();
    
    gl_Position = ciProjectionMatrix * ciViewMatrix *  ciModelMatrix    *pos2;
 
    EmitVertex();

    gl_Position = ciProjectionMatrix * ciViewMatrix * ciModelMatrix * pos3;
 
    EmitVertex();
    EndPrimitive();

}
void drawTriangleDouble(vec4 pos1,vec4 pos2,vec4 pos3)
{
	drawTriangle( pos1, pos2,pos3);
	drawTriangle( pos2,pos1,pos3);
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
    vec3 dir =  dir_in[0];
    vec3 flatl =  flat_in[0];
   mat4 rotationMatrix  = getRotationMatrix(vec3(flatl.xy,0.0),flatl.z*-1.2 );


    float base1 =3.0;
    float base2 =2.7;
	float base3 =2.0;

	float off2 =0.0*dir.z ;
    float off3 =0.04*dir.z ;
	float off4 =0.12*dir.z ;

	////////////////////////////
    vec4 pos0  ;
	pos0.x=dir.x*base1;
	pos0.y=dir.y*base1;
	pos0.z=0.0;
	pos0.w=1.0;
	  pos0= pos+pos0;

    vec4 pos1  ;
	pos1.x=-dir.x*base1;
	pos1.y=-dir.y*base1;
	pos1.w=1.0;
	pos1.z=0.0;
    pos1= pos+pos1;

	////////////////////////////
	 vec4 pos2 ;
	pos2.x=dir.x*base2+dir.y*off2;
	pos2.y=dir.y*base2+dir.x*off2;
	pos2.z=dir.z/3.0;
	pos2.w=1.0;
	pos2= pos+ rotationMatrix*pos2;


    vec4 pos3 ;
	pos3.x=-dir.x*base2+dir.y*off2;
	pos3.y=-dir.y*base2+dir.x*off2;
	pos3.z =pos2.z;
	pos3.w=1.0;
	pos3= pos+ rotationMatrix*pos3;

	////////////////////////////
	 vec4 pos4  ;
	pos4.x=dir.x*base3+dir.y*off3;
	pos4.y=dir.y*base3+dir.x*off3;
	pos4.z=dir.z/3.0 *2.0 ;
	pos4.w=1.0;
	pos4= pos+ rotationMatrix*pos4;

    vec4 pos5 ;
	pos5.x=-dir.x*base3+dir.y*off3;
	pos5.y=-dir.y*base3+dir.x*off3;
	pos5.z = pos4.z;
	pos5.w=1.0;
	pos5= pos+ rotationMatrix*pos5;

	////////////////////////////
	vec4 pos6;  
	pos6.x=dir.y*off4;
	pos6.y=dir.x*off4;
	pos6.z=dir.z;
	pos6.w=1.0;
	pos6= pos+ rotationMatrix*pos6;

    drawTriangleDouble(pos0,pos2,pos1 );
	drawTriangleDouble(pos1,pos2,pos3 );
	drawTriangleDouble(pos2,pos4,pos3 );
    drawTriangleDouble(pos3,pos4,pos5 );
    drawTriangleDouble(pos4,pos6,pos5);
    
    
    
    	
	
    
    
}
