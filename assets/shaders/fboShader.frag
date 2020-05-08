#version 130
in  vec2 texCoord;

out vec4 color;

uniform sampler2D tex;
uniform float timer;
uniform vec2 mousePos;

void main(void){
	float dist = length(gl_FragCoord.xy - mousePos)/800.;
	dist *= 2.;

	vec2 uv = gl_FragCoord.xy/vec2(800.,600.);
	color = texture2D(tex, uv/dist);
}