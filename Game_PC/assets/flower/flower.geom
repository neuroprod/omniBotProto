#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 27) out;


uniform	sampler2D	uNoiseMap;
uniform	float	time;

uniform mat4 ciModelMatrix;
uniform mat4 uShadowMatrix;

uniform mat4 ciViewMatrix;
uniform mat4 ciProjectionMatrix;
in vec3 color[];
in vec3 text[];
in vec4 posN[];
in float flatD[];
out vec3 vColor;
out vec3 vNormal;
out vec4 vShadowCoord;


const mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.5, 0.5, 0.5, 1.0 );


void drawTriangle(vec4 pos1,vec4 schadowCord1,vec4 pos2,vec4 schadowCord2,vec4 pos3,vec4 schadowCord3)
{

    vNormal = normalize(cross( normalize(pos2.xyz-pos1.xyz), normalize(pos3.xyz-pos1.xyz)));
    gl_Position = ciProjectionMatrix *pos1;
    vShadowCoord = schadowCord1;
    EmitVertex();
    
    gl_Position = ciProjectionMatrix *pos2;
    vShadowCoord = schadowCord3;
    EmitVertex();

    gl_Position = ciProjectionMatrix *pos3;
    vShadowCoord = schadowCord3;
    EmitVertex();

 
    EndPrimitive();

}


void main()
{
    
   
    
    
    float fla =flatD[0];
    float flaInv = 1.0-fla;
   
    vec4 posn = posN[0];
    vec4 pos = gl_in[0].gl_Position;
    
    
    float size =text[0].z *fla;
    
    vec2 dir = vec2(text[0].x,text[0].y);
    vec2 dirS = vec2(-dir.y,dir.x);
    
    vec2 uv = pos.xy/1000.0;
    uv.x+=time;
    uv.y+=time;
    vec2 dirWind =(texture(uNoiseMap, uv).xy - vec2(0.5,0.5) )*15*fla +dirS*2.0+dirS*flaInv*text[0].z*0.5;
   //dirWind = vec2(0);

    
    mat4 shadowMatrix = biasMatrix * uShadowMatrix * ciModelMatrix;
   
    //top
    vec4 offsetTop = vec4( dirWind*3.0 ,size, 0.0 );
    vec4 posTop = ciViewMatrix* (pos + offsetTop);
    vec4 schadowCordTop 	= ( biasMatrix * uShadowMatrix * ciModelMatrix ) * (posn + offsetTop);

    
    
    
    //base
    vec4 offsetBase1 = vec4( dir.xy* 5.0 ,0.0, 0.0 );
    vec4 posBase1 = ciViewMatrix* (pos + offsetBase1);
    vec4 schadowCordBase1 	= shadowMatrix * (posn + offsetBase1);

    
    
    vec4 offsetBase2 = vec4( dir.xy* -5.0 ,0.0, 0.0 );
    vec4 posBase2 = ciViewMatrix* (pos + offsetBase2);
    vec4 schadowCordBase2 	=  shadowMatrix* (posn + offsetBase2);
    
  
    
    vec4 offsetBase3 = vec4( dirS.xy*7.0 ,0.0, 0.0 );
    vec4 posBase3 = ciViewMatrix* (pos + offsetBase3);
    vec4 schadowCordBase3 	=  shadowMatrix * (posn + offsetBase3);
    
    
    
    
    
    //center
    vec4 offsetMid1 = vec4( dir.xy* 3.0+dirWind ,size-0.10, 0.0 );
    vec4 posMid1 = ciViewMatrix* (pos + offsetMid1);
    vec4 schadowCordMid1 	= shadowMatrix * (posn + offsetMid1);
    
    
    
    vec4 offsetMid2 = vec4( dir.xy* -3.0 +dirWind,+size-0.1, 0.0 );
    vec4 posMid2 = ciViewMatrix* (pos + offsetMid2);
    vec4 schadowCordMid2 	=  shadowMatrix* (posn + offsetMid2);
    
    
    
    vec4 offsetMid3 = vec4( dirS.xy*3.0 +dirWind,+size-0.1, 0.0 );
    vec4 posMid3 = ciViewMatrix* (pos + offsetMid3);
    vec4 schadowCordMid3 	=  shadowMatrix * (posn + offsetMid3);
    
    
    
    vec4 offsetMid4 = vec4( dirS.xy*-3.0 +dirWind,+size-0.1, 0.0 );
    vec4 posMid4 = ciViewMatrix* (pos + offsetMid4);
    vec4 schadowCordMid4	=  shadowMatrix * (posn + offsetMid3);


    //////leave
   
    vec4 offsetLeave1 = vec4( dirS.xy* 6.0+dirWind ,+size, 0.0 );
    vec4 posLeave1 = ciViewMatrix* (pos + offsetLeave1);
    vec4 schadowCordLeave1 	= shadowMatrix * (posn + offsetLeave1);
    
    
    
    vec4 offsetLeave2 = vec4( dirS.xy* -5.0 +dirWind,+size, 0.0 );
    vec4 posLeave2 = ciViewMatrix* (pos + offsetLeave2);
    vec4 schadowCordLeave2 	=  shadowMatrix* (posn + offsetLeave2);
    
    
    
    vec4 offsetLeave3 = vec4( dir.xy*-6.0 +dirWind,+size, 0.0 );
    vec4 posLeave3 = ciViewMatrix* (pos + offsetLeave3);
    vec4 schadowCordLeave3 	=  shadowMatrix * (posn + offsetLeave3);

    vec4 offsetLeave4 = vec4( dir.xy*7.0 +dirWind,+size, 0.0 );
    vec4 posLeave4 = ciViewMatrix* (pos + offsetLeave4);
    vec4 schadowCordLeave4	=  shadowMatrix * (posn + offsetLeave4);

    
    
    
    
    
     vColor =vec3(1.0,0.5,0.0);
    drawTriangle(posMid1,schadowCordMid1,posMid2,schadowCordMid2,posMid3,schadowCordMid3);
    vColor =vec3(1.0,0.5,0.0);
    drawTriangle(posMid1,schadowCordMid1,posMid2,schadowCordMid2,posMid4,schadowCordMid4);
    
     vColor =vec3(1.0,1.0,1.0);
    drawTriangle(posLeave1,schadowCordLeave1,posLeave2,schadowCordLeave2,posLeave3,schadowCordLeave3);
    
    vColor =vec3(1.0,1.0,1.0);
    drawTriangle(posLeave1,schadowCordLeave1,posLeave2,schadowCordLeave2,posLeave4,schadowCordLeave4);

    
    	
	
    
    
}
