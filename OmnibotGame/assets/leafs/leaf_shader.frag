#version 150





in vec2				vTexCoord0;


uniform	sampler2D	uGradientMap;


out vec4 			oColor;






void main( void )
{
    vec4 color = texture(uGradientMap, vTexCoord0);
    oColor =color;// texture(uDiffuseMap, vTexCoord0.st)*color ;
}
