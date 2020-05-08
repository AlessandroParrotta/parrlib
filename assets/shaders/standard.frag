#version 130

in varying vec2 texCoord;

out varying vec4 color;

uniform float timer=0.;

void main(void){
	color = vec4(pow(texCoord.x, 0.9) + pow(texCoord.y, 0.9), 1.0, 1.0, 1.0);
}