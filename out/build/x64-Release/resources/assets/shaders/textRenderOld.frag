#version 130

in vec2 texCoord;
uniform sampler2D tex;
uniform vec4 color=vec4(0.);

void main(void){
	gl_FragColor = vec4(1., 1., 1., texture2D(tex, texCoord).r) * color;
	//gl_FragColor = texture2D(tex, texCoord) * color;
	//gl_FragColor = vec4(texCoord.x, texCoord.y, 1., 1.) * color;
	//gl_FragColor = vec4(1., 1., 1., 1.);
}