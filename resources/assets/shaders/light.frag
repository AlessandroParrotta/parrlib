#version 130

in  vec2 texCoord;

out  vec4 color;

uniform sampler2D shadowMap;
uniform sampler2D tex;

uniform float timer;
uniform vec2 mousePos;
uniform vec2 lightPos;
uniform vec2 resolution;
uniform float radius;
uniform int useShadows = 1;

float sample(vec2 coord, float r){
	return step(r, texture2D(shadowMap, coord).r);
}

void main(void){
	color = vec4(vec3(1.), 1.);

	vec2 dist = gl_FragCoord.xy - lightPos;
	vec2 dir = normalize(dist);
	
	vec2 centerToRadius = dir*radius;
	
	float value = (length(dist)/length(centerToRadius));
	float modifier = 1.;//(pow(length(dist)/6.-1., 3.));

	float finalValue = value*modifier;

	color *= vec4(1.-finalValue, 1.-finalValue, 1.-finalValue, 1.-finalValue);

	if(useShadows == 1){
		//SHADOW MAP
		vec2 norm = texCoord.st * 2. - 1.;
		float theta = atan(norm.y, norm.x);
		float r = length(norm);
		float coord = (theta + 3.1415) / (2.0*3.1415);

		vec2 tc = vec2(coord, 0.0);

		float center = sample(tc, r);

		//we multiply the blur amount by our distance from center
		//this leads to more blurriness as the shadow "fades away"
		float blur = (1./resolution.x)  * smoothstep(0., 1., r); 
	
		//now we use a simple gaussian blur
		float sum = 0.0;
	
		sum += sample(vec2(tc.x - 4.0*blur, tc.y), r) * 0.05;
		sum += sample(vec2(tc.x - 3.0*blur, tc.y), r) * 0.09;
		sum += sample(vec2(tc.x - 2.0*blur, tc.y), r) * 0.12;
		sum += sample(vec2(tc.x - 1.0*blur, tc.y), r) * 0.15;
	
		sum += center * 0.16;
	
		sum += sample(vec2(tc.x + 1.0*blur, tc.y), r) * 0.15;
		sum += sample(vec2(tc.x + 2.0*blur, tc.y), r) * 0.12;
		sum += sample(vec2(tc.x + 3.0*blur, tc.y), r) * 0.09;
		sum += sample(vec2(tc.x + 4.0*blur, tc.y), r) * 0.05;

		color *= vec4(sum * smoothstep(1.0,0.0,r));
	}

	//float dist = length(gl_FragCoord.xy - lightPos);
	//dist /= 3.;
	//dist /= pow(dist, 1.6);

	//color = vec4(dist, dist, dist, 1.);
}