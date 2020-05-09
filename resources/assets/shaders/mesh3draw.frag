#version 330

in vec3 outNorm;
in vec4 outCol;
in vec2 outUv;

uniform bool textured;
uniform sampler2D tex;
uniform vec3 dirlight;

out vec4 color;

void main(void){
//	color = vec4(outNorm,1.);
//	color = vec4(outUv,0.,1.);
//	color = outCol;

	vec3 lightCol = vec3(1.) * dot(dirlight,outNorm);

	vec4 outLightCol = outCol*vec4(lightCol,1.)*10.;

	if(textured) color = texture2D(tex, outUv)*outLightCol;
	else color = outLightCol;
//	else color = vec4(outUv,0.f,1.f);
}
