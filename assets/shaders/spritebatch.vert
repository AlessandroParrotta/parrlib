#version 330

layout(location = 0) in vec4 coords;

out vec2 texCoord;

uniform mat3 transform;

void main(void){
	vec2 pos = (transform * vec3(coords.xy, 1.)).xy;
	gl_Position = vec4(pos.xy, 0., 1.);

	texCoord = coords.zw;
}