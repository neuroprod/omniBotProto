#version 150





in vec2				vTexCoord0;
in vec3				normal;

uniform	sampler2D	uGradientMap;
uniform samplerCube uIrradianceMap;

out vec4 			oColor;






void main( void )
{
    
	vec3 albedo =pow(texture(uGradientMap, vTexCoord0).xyz, vec3(2.2));
	vec3 irradiance = texture(uIrradianceMap, normal).xyz;
	vec3 diffuse = irradiance * albedo;
	 vec3 color =  diffuse;

	 color = max( vec3(0), color - 0.004);
    color = (color*(6.2*color + .5)) / (color*(6.2*color+1.7) + 0.06);
	 
	 oColor.xyz =color;
	 oColor.w =1.0;
}
