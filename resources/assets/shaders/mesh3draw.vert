#version 330

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec4 col;
layout(location = 3) in vec2 uv;

out vec3 outNorm;
out vec4 outCol;
out vec2 outUv;

uniform mat4 transform;
uniform mat4 rotm;

void main(void){
	gl_Position = transform * vec4(pos, 1.);

	outNorm = (rotm*vec4(norm, 1.)).xyz;
	outCol = col;
	outUv = uv;
}
