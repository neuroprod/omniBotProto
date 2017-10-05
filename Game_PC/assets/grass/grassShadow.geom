#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;


uniform	sampler2D	uNoiseMap;
uniform	float	time;

uniform mat4 ciProjectionMatrix;
uniform mat4 ciViewMatrix;
in vec3 color[];
in vec3 text[];

in float flatD[];

out vec3 vColor;

void main()
{
    float fla =flatD[0];
    float flaInv = 1.0-fla;
    vColor =color[0];
    
    
    vec4 pos = gl_in[0].gl_Position;
    
    
    
    
    
    
    float size =text[0].z*fla;
    vec2 dir = vec2(text[0].x,text[0].y);
    vec2 dirS = vec2(-dir.y,dir.x);
    vec2 uv = pos.xy/1000.0;
    uv.x+=time;
    uv.y+=time;
    vec2 dirWind = (texture(uNoiseMap, uv).xy - vec2(0.5,0.5) )*15*fla +dirS*2.0+dirS*flaInv*text[0].z*0.5;
   // dirWind = vec2(0);
    
    //top
    vec4 offsetTop = vec4( dirWind*3.0 ,size, 0.0 );
    vec4 posTop = ciProjectionMatrix *ciViewMatrix* (pos + offsetTop);
    
    
    
    
    //base
    vec4 offsetBase1 = vec4( dir.xy* 5.0 ,0.0, 0.0 );
    vec4 posBase1 = ciProjectionMatrix *ciViewMatrix* (pos + offsetBase1);
   
    
    vec4 offsetBase2 = vec4( dir.xy* -5.0 ,0.0, 0.0 );
    vec4 posBase2 = ciProjectionMatrix *ciViewMatrix* (pos + offsetBase2);
    
    
    
    vec4 offsetBase3 = vec4( dirS.xy*7.0 ,0.0, 0.0 );
    vec4 posBase3 = ciProjectionMatrix *ciViewMatrix* (pos + offsetBase3);
   
    
    
    
    //mid
    vec4 offsetMid1 = vec4( dir.xy* 3.0+dirWind ,+size/2.0, 0.0 );
    vec4 posMid1 = ciProjectionMatrix *ciViewMatrix* (pos + offsetMid1);
    
    
    
    vec4 offsetMid2 = vec4( dir.xy* -3.0 +dirWind,+size/2.0, 0.0 );
    vec4 posMid2 = ciProjectionMatrix *ciViewMatrix* (pos + offsetMid2);
   
    
    
    vec4 offsetMid3 = vec4( dirS.xy*5.0 +dirWind,+size/2.0, 0.0 );
    vec4 posMid3 = ciProjectionMatrix *ciViewMatrix* (pos + offsetMid3);
   
    
    
    
    
    

    
    EndPrimitive();

    
    
    
    
    
    // vColor =vec3(1,0,0);
    
    gl_Position = posBase1;
       EmitVertex();
    
    gl_Position = posBase2;
    EmitVertex();
    
    gl_Position = posMid1;
    EmitVertex();
    
    gl_Position = posMid2;
    EmitVertex();
    
    gl_Position = posTop;
    EmitVertex();
    
    EndPrimitive();
    //////////////////////////////////////
    //vColor =vec3(0,1,0);
    gl_Position = posBase3;
   
    EmitVertex();
    
    
    gl_Position = posBase1;
   
    EmitVertex();
    
    gl_Position = posMid3;

    EmitVertex();
    
    
    gl_Position = posMid1;
  
    EmitVertex();
    
    
    gl_Position = posTop;
   
    EmitVertex();
    EndPrimitive();
    
    //////////////////////////////////
    //vColor =vec3(0,0,1);
    gl_Position = posBase3;

    EmitVertex();
    
    gl_Position = posBase2;

    EmitVertex();
    
    gl_Position = posMid3;
  
    EmitVertex();
    
    gl_Position = posMid2;
 
    EmitVertex();
    
    
    
    
    gl_Position = posTop;
   
    EmitVertex();
    EndPrimitive();
}
