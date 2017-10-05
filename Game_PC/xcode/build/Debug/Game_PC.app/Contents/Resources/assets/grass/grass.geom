#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 27) out;


uniform	sampler2D	uNoiseMap;
uniform	float	time;

uniform mat4 ciModelMatrix;
uniform mat4 uShadowMatrix;

uniform mat4 ciViewMatrix;
uniform mat4 ciProjectionMatrix;

uniform vec3 lightPos;

in vec3 color[];
in vec3 text[];
in vec4 posN[];
in float flatD[];
out vec3 vColor;
out vec3 vNormal;
out float r;
out vec4 vShadowCoord;


const mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.5, 0.5, 0.5, 1.0 );


void drawTriangle(vec4 pos1,vec4 schadowCord1,vec4 pos2,vec4 schadowCord2,vec4 pos3,vec4 schadowCord3)
{
    
    vec3 dir =normalize( pos1.xyz-lightPos);
    

    vNormal = normalize(cross( pos2.xyz-pos1.xyz, pos3.xyz-pos1.xyz));
    
    r = dot(dir,vNormal)*0.25 +0.75;
    
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
    
    vColor =color[0];
    
    
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
    float s =1.0;
    //top
    vec4 offsetTop = vec4( dirWind*3.0 ,size, 0.0 )*s;
    vec4 posTop = ciViewMatrix* (pos + offsetTop);
    vec4 schadowCordTop 	= ( biasMatrix * uShadowMatrix * ciModelMatrix ) * (posn + offsetTop);

    
    
    
    //base
    vec4 offsetBase1 = vec4( dir.xy* 5.0 ,0.0, 0.0 )*s;
    vec4 posBase1 = ciViewMatrix* (pos + offsetBase1);
    vec4 schadowCordBase1 	= shadowMatrix * (posn + offsetBase1);

    
    
    vec4 offsetBase2 = vec4( dir.xy* -5.0 ,0.0, 0.0 )*s;
    vec4 posBase2 = ciViewMatrix* (pos + offsetBase2);
    vec4 schadowCordBase2 	=  shadowMatrix* (posn + offsetBase2);
    
  
    
    vec4 offsetBase3 = vec4( dirS.xy*7.0 ,0.0, 0.0 )*s;
    vec4 posBase3 = ciViewMatrix* (pos + offsetBase3);
    vec4 schadowCordBase3 	=  shadowMatrix * (posn + offsetBase3);
    
    
    
    
    
    //mid
    vec4 offsetMid1 = vec4( dir.xy* 3.0+dirWind ,+size/2.0, 0.0 )*s;
    vec4 posMid1 = ciViewMatrix* (pos + offsetMid1);
    vec4 schadowCordMid1 	= shadowMatrix * (posn + offsetMid1);
    
    
    
    vec4 offsetMid2 = vec4( dir.xy* -3.0 +dirWind,+size/2.0, 0.0 )*s;
    vec4 posMid2 = ciViewMatrix* (pos + offsetMid2);
    vec4 schadowCordMid2 	=  shadowMatrix* (posn + offsetMid2);
    
    
    
    vec4 offsetMid3 = vec4( dirS.xy*5.0 +dirWind,+size/2.0, 0.0 )*s;
    vec4 posMid3 = ciViewMatrix* (pos + offsetMid3);
    vec4 schadowCordMid3 	=  shadowMatrix * (posn + offsetMid3);

    
    
    
    
    
    
    
    
    
    
    drawTriangle(posBase1,schadowCordBase1,posMid1,schadowCordMid1,posBase2,schadowCordBase2);
    drawTriangle(posBase2,schadowCordBase2,posMid2,schadowCordMid2,posMid1,schadowCordMid1);
     drawTriangle(posMid1,schadowCordMid1,posMid2,schadowCordMid2,posTop,schadowCordTop);
   
    
  drawTriangle(posBase3,schadowCordBase3, posBase1,schadowCordBase1,posMid3,schadowCordMid3);
    drawTriangle(posBase1,schadowCordBase1,posMid1,schadowCordMid1,posMid3,schadowCordMid3);
   drawTriangle(posMid3,schadowCordMid3,posMid1,schadowCordMid1,posTop,schadowCordTop);
 
  drawTriangle(posBase3,schadowCordBase3,posMid2,schadowCordMid2,posBase2,schadowCordBase2);
   drawTriangle(posBase3,schadowCordBase3,posMid3,schadowCordMid3,posMid2,schadowCordMid2);
    drawTriangle(posMid3,schadowCordMid3,posTop,schadowCordTop,posMid2,schadowCordMid2);
   
    /*
    gl_Position = posBase1;
    vShadowCoord = schadowCordBase1;
    EmitVertex();

    
    gl_Position = posBase2;
    vShadowCoord = schadowCordBase2;
    EmitVertex();
    
    
    gl_Position = posMid1;
    vShadowCoord = schadowCordMid1;
    EmitVertex();
    
    
    gl_Position = posMid2;
    vShadowCoord = schadowCordMid2;
    EmitVertex();
    
    
    gl_Position = posTop;
    vShadowCoord = schadowCordTop;
    EmitVertex();
    EndPrimitive();
    //////////////////////////////////////
    
    gl_Position = posBase3;
    vShadowCoord = schadowCordBase3;
    EmitVertex();
    
    
    gl_Position = posBase1;
    vShadowCoord = schadowCordBase1;
    EmitVertex();
    
    gl_Position = posMid3;
    vShadowCoord = schadowCordBase3;
    EmitVertex();
    
    
    gl_Position = posMid1;
    vShadowCoord = schadowCordBase1;
    EmitVertex();

    
    gl_Position = posTop;
    vShadowCoord = schadowCordTop;
    EmitVertex();
    EndPrimitive();

    //////////////////////////////////
   
    gl_Position = posBase3;
    vShadowCoord = schadowCordBase3;
    EmitVertex();
    
    gl_Position = posBase2;
    vShadowCoord = schadowCordBase2;
    EmitVertex();
    
    gl_Position = posMid3;
    vShadowCoord = schadowCordBase3;
    EmitVertex();
    
    gl_Position = posMid2;
    vShadowCoord = schadowCordBase2;
    EmitVertex();

   
    
    
    gl_Position = posTop;
    vShadowCoord = schadowCordTop;
    EmitVertex();
    EndPrimitive();

    
    
    */
    
    
    
    
    
    
    
    	
	
    
    
}
