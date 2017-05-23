#version 150 core

uniform vec3 uLightPos;
uniform sampler2DShadow uShadowMap;

in vec4 vColor;
in vec4 vPosition;
in vec3 vNormal;
in vec4 vShadowCoord;

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


float samplePCF4x4( vec4 sc )
{
    const int r = 1;
    const int s = 2 * r;
    
    float shadow = 0.0;
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s,-s) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r,-s) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( r,-s) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( s,-s) );
    
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s,-r) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r,-r) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( r,-r) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( s,-r) );
    
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s, r) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r, r) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( r, r) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( s, r) );
    
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-s, s) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2(-r, s) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( r, s) );
    shadow += textureProjOffset( uShadowMap,  sc, ivec2( s, s) );
    
    return shadow/16.0;
}


void main( void )
{
	vec3 Normal			= normalize( vNormal );
	vec3 LightVec		= normalize( uLightPos - vPosition.xyz );
	float NdotL			= max( dot( vNormal, LightVec ), 0.0 );
	
	vec3 Diffuse		= vec3( NdotL );
	vec3 Ambient		= vec3( 0.3 );
	
	vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
	float Shadow		= 1.0;
	
	if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) {
        Shadow = samplePCF3x3(ShadowCoord) ;//textureProj( uShadowMap, ShadowCoord, -0.00005 );
	}
    Shadow =clamp(Shadow,0.2,1.0);
//Color.rgb = vec3(1.0,1.0,1.0)*Shadow;
	Color.rgb = vec3(0.08,0.08,0.00)*Shadow;
	Color.a	= 1.0;
}
