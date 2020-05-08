
in varying vec2 texCoord;

out varying vec4 color;

uniform sampler2D tex;

void main(void){
	vec2 uv = gl_FragCoord.xy/iResolution.xy;
	color = texture2D(tex, uv);
}