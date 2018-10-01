#version 410

layout(location = 0) in vec2 fragUv;
layout(location = 1) in vec3 pos;
layout(location = 2) in vec3 norm;

layout(location = 3) noperspective in vec3 dista;

layout(location = 4) in vec4 vColor;

layout(location = 0) out vec4 fragmentColor;

//layout(location = 1) out vec4 red;
uniform sampler2D textureIN;
uniform vec3 selectedColor;

vec3 lightColor = vec3(1, 1, 1);
vec4 lPos = vec4(0, 10, 0, 1000);
vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

void main()
{

	float nearD = min(min(dista[0],dista[1]),dista[2]);
	float edgeIntensity = exp2(-1.0*nearD*nearD);

	//vec4 color = vec4(selectedColor, 1.0);// texture(textureIN, vec2(fragUv.x, fragUv.y));
	vec4 color = vColor;
	fragmentColor = color; //vec4(fragUv.x, fragUv.y, 0, 1);
//	red = vec4(1.0, 0.0, 0, 1);

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
	
	//fragmentColor *= letThereBeLight;
	
	fragmentColor = (edgeIntensity * vec4(0.1,0.1,0.1,1.0)) + ((1.0-edgeIntensity) * (color * letThereBeLight));
	//fragmentColor = (color * letThereBeLight);
	//fragmentColor = vec4(edgeIntensity, edgeIntensity, edgeIntensity, 1.0);
	//fragmentColor = vec4(nearD, nearD, nearD , 1.0);
}

void func(out vec3 foo, out vec4 bar)
{
	// dostuff();
}