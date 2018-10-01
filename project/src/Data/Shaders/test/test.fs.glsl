#version 410

layout(location = 0) in vec2 UV;

layout(location = 0) out vec4 fragmentColor;

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

	Texture t = getTexel(UV);
		
	fragmentColor = process(t, UV, ivec2(8,8));
	//fragmentColor =mix(vec4(0.0), vec4(1.0), diagonal4(UV, 0.0));
}
