#version 150





in vec2				vTexCoord0;
in vec4 vShadowCoord;

uniform	sampler2D	uGradientMap;
uniform sampler2DShadow uShadowMap;

out vec4 			oColor;


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
    vec4 color = texture(uGradientMap, vTexCoord0);
    
    vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
    float Shadow		= 1.0;
    if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) {
        Shadow = samplePCF3x3(ShadowCoord);//textureProj( uShadowMap, ShadowCoord, -0.00005 );
    }
    Shadow =clamp(Shadow,0.7,1.0);
    
    color.xyz*=Shadow;
   
    oColor =color;// texture(uDiffuseMap, vTexCoord0.st)*color ;
}
