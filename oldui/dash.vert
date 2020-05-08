#version 450

layout(location = 0) in vec2 coord;
layout(location = 1) in vec4 color;

varying out vec4 outColor;

uniform float ortho[4];

void main(){

	vec2 pos = vec2(0.);
	pos.x = (coord.x-ortho[0])/(ortho[1]-ortho[0])*2.-1.;
	pos.y = (coord.y-ortho[2])/(ortho[3]-ortho[2])*2.-1.;

	gl_Position = vec4(pos, 0., 1.);
	outColor = color;
}