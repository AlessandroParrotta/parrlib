#version 130

in  vec2 texCoord;

out  vec4 color;

uniform sampler2D occluder;
uniform float timer;
uniform vec2 resolution;

void main(void){
	
	float distance = 1.;

	for(float y=0.; y<resolution.y; y+= 1.0){
		vec2 norm = vec2(texCoord.s, y/resolution.y)*2. - 1.;

		float theta = 3.1415*1.5 + norm.x*3.1415;
		float r = (1.0 + norm.y)*0.5;

		vec2 coord = vec2(-r * sin(theta), -r * cos(theta))/2. + 0.5;

		vec4 data = texture2D(occluder, coord);

		float dst = y/resolution.y;

		float caster = data.a;
		if(caster > 0.75){
			distance = min(distance, dst);
		}
	}

	color = vec4(vec3(distance), 1.);
}