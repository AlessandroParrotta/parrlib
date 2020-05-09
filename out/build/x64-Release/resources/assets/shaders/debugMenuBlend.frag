#version 130

in  vec2 texCoord;

out  vec4 color;

uniform sampler2D tex;

uniform vec2 windowSize;
uniform vec2 screenSize;

void main(void){
	vec2 uv = gl_FragCoord.xy/windowSize;
	
	color = texture2D(tex, texCoord);
}
