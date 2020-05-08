#version 330

layout(location = 0) in vec4 coords;
layout(location = 1) in vec4 color;

out vec2 texCoord;
out vec4 outColor;

uniform mat3 transform;
uniform vec2 atlasSize;

void main(void){
	vec2 pos = (transform * vec3(coords.xy, 1.)).xy;
	gl_Position = vec4(pos.xy, 0., 1.);

	texCoord = vec2(coords.z, coords.w)/atlasSize;
//	texCoord = vec2(coords.z, coords.w)/atlasSize - (1./atlasSize)*(coords.zw/atlasSize);
//	texCoord = coords.zw;

	outColor = color;
}