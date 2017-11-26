#version 410

layout(location = 0) in vec2 fragUv;
layout(location = 1) in vec3 pos;
layout(location = 2) in vec3 norm;

layout(location = 0) out vec4 fragmentColor;
layout(location = 1) out vec4 normalColor;
layout(location = 2) out vec4 worldPos;

uniform float time;
uniform vec3 cameraPos;
uniform vec3 eyeDir;

uniform vec3 sunMoonDir;

const float PI = 3.1415;

const vec3 sunDir = vec3(0.47, -0.48, -0.73);

//[0.135633, 0.577581, 1.000000, 1.000000]
const vec3 daySky = vec3(0.3, 0.55, 0.8);
//[0.024422, 0.088654, 0.147314, 1.000000]
const vec3 nightSky = vec3(0.024422, 0.088654, 0.147314);

//[1.000000, 0.924311, 0.589900, 1.000000]
const vec3 sun = vec3(1.0, 0.7, 0.4);
const vec3 sun2 = vec3(1.0, 0.8, 0.5);

const vec3 moon = vec3(0.5, 0.5, 0.5);
const vec3 moon2 = vec3(0.5, 0.5, 0.5);


vec3 sky(in vec3 light, in vec3 rd) {
    vec3 col = vec3(0);
         
	if(rd.y < 0.0){
	// Sky with haze		
	float si = sin(time * PI);
	
	vec3 sky = mix(daySky, nightSky, si);
	col = sky * (1.0 - 0.8 * rd.y) * 0.9;
			
	vec3 s = mix(sun, moon, si);
	vec3 s2 = mix(sun2, moon2, si);
	
	// Sun
	float sundot = clamp(dot(rd, light), 0.0, 1.0);
	col += 0.25 * s * pow(sundot, 16.0);
	col += 0.75 * s2 * pow(sundot, 128.0);

    // Horizon/atmospheric perspective
	vec3 persp = mix(vec3(0.7, 0.75, 0.8), nightSky, si);
	
    col = mix(col, persp, pow(1.0 - max(abs(rd.y), 0.0), 8.0));
    }
	
	return col;
}

void main()
{
	vec3 light = normalize(sunMoonDir);	
	
	fragmentColor = vec4(sky(light, normalize(cameraPos - pos)), 1.0); 
	normalColor = vec4(norm, 1.0);
	worldPos = vec4(pos, 1.0);
}
