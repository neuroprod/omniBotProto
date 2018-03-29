#version 150


in vec3 vNormal;

out vec4 outColor;
uniform samplerCube uIrradianceMap;


void main()
{
    vec3 albedo = pow(vec3(0.2,0.7,0.1), vec3(2.2));
	vec3 irradiance = texture(uIrradianceMap, vNormal.xzy).xyz;


   vec3 diffuse = irradiance * albedo;
	vec3 color =  diffuse;//*   shadow;

	color = max( vec3(0), color - 0.004);
    color = (color*(6.2*color + .5)) / (color*(6.2*color+1.7) + 0.06);
	


    outColor = vec4( color, 1.0);
  
}
