#version 330

in vec2 texCoord;

out vec4 color;

uniform sampler2D tex;

void main(void){
	color = texture2D(tex, texCoord);
}

