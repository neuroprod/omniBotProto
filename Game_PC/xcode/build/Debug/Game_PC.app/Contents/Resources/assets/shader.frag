#version 150





in vec2				vTexCoord0;
in float vDarknes;
in float vColor;
in vec4 vShadowCoord;
uniform	sampler2D	uDiffuseMap;

uniform	sampler2D	uGradientMap;
uniform sampler2DShadow uShadowMap;
out vec4 			oColor;

void main( void )
{
    vec4 color = texture(uGradientMap, vec2(vColor,vDarknes));
    
    vec4 ShadowCoord	= vShadowCoord / vShadowCoord.w;
    float Shadow		= 1.0;
    if ( ShadowCoord.z > -1 && ShadowCoord.z < 1 ) {
        Shadow = textureProj( uShadowMap, ShadowCoord, -0.00005 );
    }
    Shadow =clamp(Shadow,0.5,1.0);
    oColor =color*Shadow;// texture(uDiffuseMap, vTexCoord0.st)*color ;
}
