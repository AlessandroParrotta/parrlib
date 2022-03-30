#version 330

uniform sampler2D tex;

in vec4 outCol;
in vec2 outTex;

out vec4 col;

void main(void){
	col = texture2D(tex,outTex)*outCol;
}