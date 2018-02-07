#version 410

layout(location = 0) in vec2 fragUv;

layout(location = 0) out vec4 fragmentColor;

uniform sampler2D textureDiff;
uniform sampler2D textureNorm;
uniform sampler2D textureWPos;
uniform sampler2D textureShadow;

uniform vec3 eyePos;

uniform mat4 depthMVP;

vec3 lightColor = vec3(1, 1, 1);
vec4 lPos = vec4(0, 10, 0, 1000);
vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

void main()
{

	float shading = 1.0;

	vec4 color = texture(textureDiff, fragUv);
	vec3 norm  = texture(textureNorm, fragUv).xyz;
	vec3 pos   = texture(textureWPos, fragUv).xyz;
	
	vec4 shadowPos = depthMVP * vec4(pos, 1.0);
	
	vec3 shad = texture(textureShadow, vec2(shadowPos.x, shadowPos.y) ).xyz;
	
	vec3 sPos = texture(textureShadow, fragUv).xxx;
	
	vec4 letThereBeLight = vec4(0);

	float dist = distance(pos.xyz, lPos.xyz);
	
	float fr = 1;
	
	if(distance(pos.xyz, eyePos) < 50 )
	{
		fr = dot(normalize(eyePos - pos), norm);
	}
	
	if( dist < 900.0)
	{
		//if(shad.r < shadowPos.z)
		//{
		//	shading = 0.5;
		//}
	
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
	}
	else
	{
		letThereBeLight = vec4(1.0);
	}
	//letThereBeLight = clamp(letThereBeLight, 0.2, 1.0);
	//vec4(vec3(shading), 1.0);
	//vec4(sPos, 1.0);
	
	//color = mix(vec4(1, 0, 0, 0), color, fr);
	
	fragmentColor =  color;//(color * letThereBeLight * shading); // pow(color, vec4(vec3(0.4545), 1.0));
}
