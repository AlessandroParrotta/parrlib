#version 330

layout(location = 0) in vec2 pos;
layout(location = 1) in vec4 col;
layout(location = 2) in vec2 tex;

out vec4 outCol;
out vec2 outTex;

void main(void){
	gl_Position = vec4(pos, 0., 1.);
	outCol = col;
	outTex = tex;
}