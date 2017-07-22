#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 15) out;


uniform	sampler2D	uNoiseMap;
uniform	float	time;

uniform mat4 ciModelMatrix;
uniform mat4 uShadowMatrix;

uniform mat4 ciProjectionMatrix;
in vec3 color[];
in vec3 text[];
in vec4 posN[];
in float flatD[];
out vec3 vColor;
out vec4 vShadowCoord;


const mat4 biasMatrix = mat4( 0.5, 0.0, 0.0, 0.0,
                             0.0, 0.5, 0.0, 0.0,
                             0.0, 0.0, 0.5, 0.0,
                             0.5, 0.5, 0.5, 1.0 );


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
    vec2 dirWind = (texture(uNoiseMap, uv).xy - vec2(0.5,0.5) )*15*fla +dirS*2.0+dirS*flaInv*text[0].z*0.5;
    

    
    mat4 shadowMatrix = biasMatrix * uShadowMatrix * ciModelMatrix;
    
    //top
    vec4 offsetTop = vec4( dirWind*3.0 ,size, 0.0 );
    vec4 posTop = ciProjectionMatrix * (pos + offsetTop);
    vec4 schadowCordTop 	= ( biasMatrix * uShadowMatrix * ciModelMatrix ) * (posn + offsetTop);

    
    
    
    //base
    vec4 offsetBase1 = vec4( dir.xy* 5.0 ,0.0, 0.0 );
    vec4 posBase1 = ciProjectionMatrix * (pos + offsetBase1);
    vec4 schadowCordBase1 	= shadowMatrix * (posn + offsetBase1);

    
    
    vec4 offsetBase2 = vec4( dir.xy* -5.0 ,0.0, 0.0 );
    vec4 posBase2 = ciProjectionMatrix * (pos + offsetBase2);
    vec4 schadowCordBase2 	=  shadowMatrix* (posn + offsetBase2);
    
  
    
    vec4 offsetBase3 = vec4( dirS.xy*7.0 ,0.0, 0.0 );
    vec4 posBase3 = ciProjectionMatrix * (pos + offsetBase3);
    vec4 schadowCordBase3 	=  shadowMatrix * (posn + offsetBase3);
    
    
    
    
    
    //mid
    vec4 offsetMid1 = vec4( dir.xy* 3.0+dirWind ,+size/2.0, 0.0 );
    vec4 posMid1 = ciProjectionMatrix * (pos + offsetMid1);
    vec4 schadowCordMid1 	= shadowMatrix * (posn + offsetMid1);
    
    
    
    vec4 offsetMid2 = vec4( dir.xy* -3.0 +dirWind,+size/2.0, 0.0 );
    vec4 posMid2 = ciProjectionMatrix * (pos + offsetMid2);
    vec4 schadowCordMid2 	=  shadowMatrix* (posn + offsetMid2);
    
    
    
    vec4 offsetMid3 = vec4( dirS.xy*5.0 +dirWind,+size/2.0, 0.0 );
    vec4 posMid3 = ciProjectionMatrix * (pos + offsetMid3);
    vec4 schadowCordMid3 	=  shadowMatrix * (posn + offsetMid3);

    
    
    
    
    
    
    
    
    
    
    
   //vColor =vec3(1,0,0);
    
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
    // vColor =vec3(0,1,0);
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
    // vColor =vec3(0,0,1);
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

    
    
    
    
    
    
    
    
    
    
    	
	
    
    
}
