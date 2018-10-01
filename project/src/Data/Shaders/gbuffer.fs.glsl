#version 410

layout(location = 0) in vec2 fragUv;
layout(location = 1) in vec3 pos;
layout(location = 2) in vec3 norm;

layout(location = 3) noperspective in vec3 dista;

layout(location = 4) in vec4 vColor;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 worldPos;

//layout(location = 1) out vec4 red;
uniform sampler2D textureIN;
uniform vec3 selectedColor;

vec3 lightColor = vec3(1, 1, 1);
vec4 lPos = vec4(0, 10, 0, 1000);
vec4 ambient = vec4(0.2, 0.2, 0.2, 1);

struct Texture
{
int func;
int p1;
int p2;
int p3;
float zValue;
};

Texture tex[16] = Texture[16](
Texture(4, 255, 000, 000, 0.0), 
Texture(3, 255, 000, 000, 0.0),
Texture(2, 000, 255, 000, 0.0), 
Texture(4, 000, 255, 000, 0.0),
Texture(3, 255, 000, 000, 0.0), 
Texture(0, 255, 000, 000, 0.0),
Texture(1, 000, 255, 000, 0.0), 
Texture(2, 000, 255, 000, 0.0),
Texture(1, 000, 000, 255, 0.0),
Texture(2, 000, 000, 255, 0.0),
Texture(3, 255, 255, 255, 0.0),
Texture(0, 255, 255, 255, 0.0),
Texture(4, 000, 000, 255, 0.0), 
Texture(1, 000, 000, 255, 0.0),
Texture(0, 255, 255, 255, 0.0), 
Texture(4, 255, 255, 255, 0.0)
);


int toIndex(ivec2 arrIdx, ivec2 size)
{
	return (arrIdx.x * size.x) + arrIdx.y;
}

Texture getTexel(vec2 uv)
{
	ivec2 size = ivec2(4, 4);

	ivec2 arrIdx = ivec2(int(uv.x * float(size.x)), int(uv.y * float(size.y)));
	
	return tex[toIndex(arrIdx, size)];
}

float diagonal1(vec2 uv, float offset)
{
	float val = (uv.x + uv.y) + offset;
	return floor(val);
}

float diagonal2(vec2 uv, float offset)
{
	float val = (uv.x - uv.y) + offset;
	return 1.0 + floor(val);
}

float diagonal3(vec2 uv, float offset)
{
	float val = (uv.x - uv.y) + offset;
	return -floor(val);
}

float diagonal4(vec2 uv, float offset)
{
	float val = (uv.x + uv.y) + offset;
	return 1.0 - floor(val);
}

vec2 reposition(vec2 uv, ivec2 size)
{
	
	vec2 div = 1.0 / vec2(size);
	
	return (mod(uv, div) / div);
}

vec4 process(Texture t, vec2 uv, ivec2 tSize)
{
	vec4 color = vec4(float(t.p1) / 255.0, float(t.p2) / 255.0, float(t.p3) / 255.0, 1.0);
	int func = t.func;

	vec2 tuv = reposition(uv, tSize);
	
	switch(func)
	{
		case 0:
			color = mix(vec4(0.0), color, diagonal1(tuv, 0.0));
		break;
		
		case 1:
			color = mix(vec4(0.0), color, diagonal2(tuv, 0.0));
		break;
		
		case 2:
			color = mix(vec4(0.0), color, diagonal3(tuv, 0.0));
		break;
		
		case 3:
			color = mix(vec4(0.0), color, diagonal4(tuv, 0.0));
		break;
		
		default:
			color = color;
	}
	
	return color;
}


void main()
{

	float nearD = min(min(dista[0],dista[1]),dista[2]);
	float edgeIntensity = exp2(-1.0*nearD*nearD);

	vec4 color = vColor;
	
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
	
	fragmentColor = (edgeIntensity * vec4(0.1,0.1,0.1,1.0)) + ((1.0-edgeIntensity) * (color * letThereBeLight));
	
	Texture t = getTexel(fragUv);
		
	fragmentColor = process(t, fragUv, ivec2(8,8));
	
	fragmentColor = color;
	normalColor = vec4(norm, 1.0);
	worldPos = vec4(pos, 1.0);
}
