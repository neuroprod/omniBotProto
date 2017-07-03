#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 5) out;


uniform	sampler2D	uNoiseMap;
uniform	float	time;

uniform mat4 ciProjectionMatrix;
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
    vec2 dirWind =(texture(uNoiseMap, uv).xy - vec2(0.5,0.5) )*15*fla +dirS+dirS*flaInv*5;
    

    
    gl_Position = ciProjectionMatrix * (pos+vec4( dir.x* 2,0,dir.y*2 ,0) );
    EmitVertex();
	
    gl_Position = ciProjectionMatrix * (pos+vec4( dir.x* -2,0,dir.x* -2,0) );
    EmitVertex();
   
    
    
    gl_Position = ciProjectionMatrix * (pos+vec4( (dir.x* 1) +dirWind.x,-size/2.0,(dir.y)+dirWind.y,0 ) );
    EmitVertex();
    
    gl_Position = ciProjectionMatrix * (pos+vec4( (dir.x*-1)+dirWind.x ,-size/2.0,(dir.y*-1)+dirWind.y ,0 ) );
    EmitVertex();

    vec4 offset4 = vec4(dirWind.x*3 ,-size,dirWind.y*3, 0.0 );
    gl_Position = ciProjectionMatrix * (gl_in[0].gl_Position + offset4);
    EmitVertex();
   
	
	EndPrimitive();
}
