#version 150




in vec4 vShadowCoord;
in vec2				vTexCoord0;
in vec3				normal;

uniform	sampler2D	uGradientMap;
uniform samplerCube uIrradianceMap;

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
    vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
	float shadow		= 1.0;
	
	if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) {
      shadow = samplePCF3x3(ShadowCoord) ;
	}
    shadow =clamp(shadow,0.7,1.0);


	vec3 albedo =pow(texture(uGradientMap, vTexCoord0).xyz, vec3(2.2));
	vec3 irradiance = texture(uIrradianceMap, normal).xyz;
	vec3 diffuse = irradiance * albedo;
	vec3 color =  diffuse*   shadow;

	color = max( vec3(0), color - 0.004);
    color = (color*(6.2*color + .5)) / (color*(6.2*color+1.7) + 0.06);
	 
	oColor.xyz =color;
	oColor.w =1.0;
}
