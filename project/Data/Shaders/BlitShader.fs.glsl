#version 410

layout(location = 0) in vec2 fragUv;

layout(location = 0) out vec4 fragmentColor;

uniform sampler2D textureDiff;
uniform sampler2D textureNorm;
uniform sampler2D textureWPos;

vec3 lightColor = vec3(1, 1, 1);
vec4 lPos = vec4(0, 10, 0, 1000);
vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

void main()
{

	vec4 color = texture(textureDiff, fragUv);
	vec3 norm  = texture(textureNorm, fragUv).xyz;
	vec3 pos   = texture(textureWPos, fragUv).xyz;
	
	vec4 letThereBeLight = vec4(0);

	float dist = distance(pos.xyz, lPos.xyz);
	
	float d = lPos.w;
	
	if(dist < d)
	{
		float attenuation = 1.0;
		if(dist != 0)
			attenuation = 1 - clamp((pow(dist,1.5) / d), 0, 1);
			attenuation = max(attenuation, 0);
		
		vec3 s = normalize(vec3(lPos.xyz - pos.xyz));

		vec3 r = reflect(s, norm.xyz);
	   
		letThereBeLight += vec4(lightColor.rgb * 1.5 * attenuation * max(dot(norm.xyz, s), 0), 1.0);
	
	}
	
	letThereBeLight = clamp(letThereBeLight, 0.2, 1.0);
	
	fragmentColor = color * letThereBeLight;
}
