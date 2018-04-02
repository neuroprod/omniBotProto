#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 27) out;


uniform mat4 ciModelMatrix;
uniform mat4 ciViewMatrix;
uniform mat4 ciProjectionMatrix;
in vec3 dir_in[];

out vec3 vNormal;



void drawTriangle(vec4 pos1,vec4 pos2,vec4 pos3)
{ 

    vNormal = normalize(cross( pos2.xyz-pos1.xyz, pos3.xyz-pos1.xyz));
    
    gl_Position = ciProjectionMatrix* ciViewMatrix* ciModelMatrix  *pos1;
   
    EmitVertex();
    
    gl_Position = ciProjectionMatrix * ciViewMatrix* ciModelMatrix   *pos2;
 
    EmitVertex();

    gl_Position = ciProjectionMatrix * ciViewMatrix* ciModelMatrix *pos3;
 
    EmitVertex();
    EndPrimitive();

}
void drawTriangleDouble(vec4 pos1,vec4 pos2,vec4 pos3)
{
	drawTriangle( pos1, pos2,pos3);
	drawTriangle( pos2,pos1,pos3);
}

void main()
{
    

    vec4 pos = gl_in[0].gl_Position;
    vec3 dir =  dir_in[0];
   
    float base1 =3.0;
    float base2 =2.7;
	float base3 =2.0;

	float off2 =0.028*dir.z ;
    float off3 =0.1*dir.z ;
	float off4 =0.3*dir.z ;

	////////////////////////////
    vec4 pos0  = pos;
	pos0.x+=dir.x*base1;
	pos0.y+=dir.y*base1;

    vec4 pos1  = pos;
	pos1.x+=-dir.x*base1;
	pos1.y+=-dir.y*base1;
    
	////////////////////////////
	 vec4 pos2  = pos;
	pos2.x+=dir.x*base2+dir.y*off2;
	pos2.y+=dir.y*base2+dir.x*off2;
	pos2.z+=dir.z/3.0;

    vec4 pos3  = pos;
	pos3.x+=-dir.x*base2+dir.y*off2;
	pos3.y+=-dir.y*base2+dir.x*off2;
	pos3.z = pos2.z;
    
	////////////////////////////
	 vec4 pos4  = pos;
	pos4.x+=dir.x*base3+dir.y*off3;
	pos4.y+=dir.y*base3+dir.x*off3;
	pos4.z+=dir.z/3.0 *2.0 ;

    vec4 pos5  = pos;
	pos5.x+=-dir.x*base3+dir.y*off3;
	pos5.y+=-dir.y*base3+dir.x*off3;
	pos5.z = pos4.z;

	////////////////////////////
	vec4 pos6  = pos;
	pos6.x+=dir.y*off4;
	pos6.y+=dir.x*off4;
	pos6.z+=dir.z;
 
    drawTriangleDouble(pos0,pos2,pos1);
	drawTriangleDouble(pos1,pos2,pos3);
	drawTriangleDouble(pos2,pos4,pos3);
    drawTriangleDouble(pos3,pos4,pos5);
    drawTriangleDouble(pos4,pos6,pos5);
    
    
    
    	
	
    
    
}
