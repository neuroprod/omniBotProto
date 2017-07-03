#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;


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
    
    
    
   float fla =flatD[0];
    float flaInv = 1.0-fla;
    vColor =color[0];
    vec4 posn = posN[0];
    
    vec4 pos = gl_in[0].gl_Position;
    float size =text[0].z *fla;
    vec2 dir = vec2(text[0].x,text[0].y);
    vec2 dirS = vec2(-dir.y,dir.x);
    vec2 uv = pos.xy/1000.0;
    uv.x+=time;
    uv.y+=time;
    vec2 dirWind = (texture(uNoiseMap, uv).xy - vec2(0.5,0.5) )*15*fla +dirS+dirS*flaInv*text[0].z*0.5;
    

    
    gl_Position = ciProjectionMatrix * (pos+vec4( dir.xy* 2,0,0 ) );
    vShadowCoord = ( biasMatrix * uShadowMatrix * ciModelMatrix )  * (posn+vec4( dir.xy* 2,0,0 ) );
    EmitVertex();
    
    
    
    

    gl_Position = ciProjectionMatrix * (pos+vec4( dir.xy* -2,0,0 ) );
    vShadowCoord = ( biasMatrix * uShadowMatrix * ciModelMatrix )  * (posn+vec4( dir.xy* -2,0,0 ) );
    EmitVertex();
   
    
    
    gl_Position = ciProjectionMatrix * (pos+vec4( (dir.xy* 1) +dirWind,+size/2.0,0 ) );
    vShadowCoord = ( biasMatrix * uShadowMatrix * ciModelMatrix ) * (posn+vec4( (dir.xy* 1) +dirWind,+size/2.0,0 ) );
    EmitVertex();
    
    gl_Position = ciProjectionMatrix * (pos+vec4( (dir.xy*-1)+dirWind ,+size/2.0,0 ) );
    vShadowCoord 	= ( biasMatrix * uShadowMatrix * ciModelMatrix )  * (posn+vec4( (dir.xy*-1)+dirWind ,+size/2.0,0 ) );
    EmitVertex();

    vec4 offset4 = vec4( dirWind*3.0 ,size, 0.0 );
    gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + offset4);
    vShadowCoord 	= ( biasMatrix * uShadowMatrix * ciModelMatrix ) * (posn + offset4);
    EmitVertex();
   
	
	EndPrimitive();
}
