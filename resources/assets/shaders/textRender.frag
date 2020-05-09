#version 330

in vec2 texCoord;
in vec4 outColor;

out vec4 color;

uniform sampler2D tex;

void main(void){
//	color = vec4(1.,0.,0.,1.);

//	color = texture2D(tex, texCoord);
	vec4 texCol = texture2D(tex, texCoord);
//	vec4 texCol = texelFetch(tex, ivec2(int(floor(texCoord.x)), int(floor(texCoord.y))), 0);
	color = vec4(1.,1.,1.,texCol.r)*outColor;
}

