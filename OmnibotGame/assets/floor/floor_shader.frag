#version 150 core

uniform sampler2D uFloorMap;


uniform sampler2DShadow uShadowMap;


in vec4 vShadowCoord;


in vec2	vTexCoord0;
out vec4 Color;


float samplePCF3x3( vec4 sc )
{
    const int s = 1;
    
    float shadow = 0.0;
    shadow += textureProjOffset( uShadowMap, sc, ivec2(-s,-s) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2(-s, 0) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2(-s, s) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2( 0,-s) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2( 0, 0) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2( 0, s) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2( s,-s) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2( s, 0) );
    shadow += textureProjOffset( uShadowMap, sc, ivec2( s, s) );
    return shadow/9.0;;
}




void main( void )
{
		
	vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
	float shadow		= 1.0;
	
	if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) {
      shadow = samplePCF3x3(ShadowCoord) ;
	}
    shadow =clamp(shadow,0.7,1.0);

    
     vec4 color = texture(uFloorMap, vTexCoord0);
    Color.rgb = color.xyz*shadow;
   
	Color.a	= 1.0;
}
