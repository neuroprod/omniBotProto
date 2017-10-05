#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 27) out;



uniform	float	time;
uniform mat4 ciModelMatrix;


uniform mat4 ciViewMatrix;
uniform mat4 ciProjectionMatrix;
uniform vec3 lightPos;

in vec4 color[];
out vec3 vNormal;
out float r;




void drawTriangle(vec4 pos1,vec4 pos2,vec4 pos3)
{

    
    
    vec3 dir =normalize( pos1.xyz-lightPos);
    vNormal = normalize(cross( pos2.xyz-pos1.xyz, pos3.xyz-pos1.xyz));
    r = dot(dir,vNormal)*0.25+0.75;
    
    
    
    gl_Position = ciProjectionMatrix *pos1;
   
    EmitVertex();
    
    gl_Position = ciProjectionMatrix *pos2;
    EmitVertex();

    gl_Position = ciProjectionMatrix *pos3;
  
    EmitVertex();

 
    EndPrimitive();

}


void main()
{
    
   
    
    float sTime = sin(time+color[0].w*7.0);
    float cTime =cos(time+color[0].w*7.0);
  
    vec4 pos = gl_in[0].gl_Position;
    vec3 dirT = normalize (color[0].xyz);
    vec3 dirB  =cross(dirT,vec3(0.0,0.0,1.0));
    
    vec4 offsetMid1 = vec4( dirT*5.0, 0.0 );
    vec4 posMid1 = ciViewMatrix* (pos + offsetMid1);
    
    
    
    vec4 offsetMid2 = vec4( 0.0);
    vec4 posMid2 = ciViewMatrix* (pos + offsetMid2);
    
    
    
    
    vec4 offsetMid3 = vec4( dirB*-5.0*sTime+    vec3(0.0,0.0,5.0*cTime)      -dirT*4.0, 0.0 );
    vec4 posMid3 = ciViewMatrix* (pos + offsetMid3);
    
    
    
    
  
    vec4 offsetMid4 = vec4( dirB*5.0*sTime+    vec3(0.0,0.0,5.0*cTime)       -dirT*4.0, 0.0 );
    vec4 posMid4 = ciViewMatrix* (pos + offsetMid4);
    
    
    
    
    
   
    drawTriangle(posMid1,posMid2,posMid3);
  
    
    drawTriangle(posMid1,posMid2,posMid4);
	
    
    
    
    vec4 offsetMid5 = vec4( dirB*-8.0*sTime+    vec3(0.0,0.0,8.0*cTime)      +dirT*12.0, 0.0 );
    vec4 posMid5= ciViewMatrix* (pos + offsetMid5);
    
    
      drawTriangle(posMid1,posMid3,posMid5);
    
    
    vec4 offsetMid6 = vec4( dirB*8.0*sTime+    vec3(0.0,0.0,8.0*cTime)       +dirT*12.0, 0.0 );
    vec4 posMid6 = ciViewMatrix* (pos + offsetMid6);

     drawTriangle(posMid1,posMid4,posMid6);
    
    
}
