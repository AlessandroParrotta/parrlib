#version 130

attribute vec4 coords;
 out vec2 texCoord;

uniform float ortho[4];

void main(void){
	//gl_Position = gl_Vertex;
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	//texCoord = gl_TexCoord[0].st;
	
	vec2 pos = vec2(0.);
	pos.x = (coords.x-ortho[0])/(ortho[1]-ortho[0])*2.-1.;
	pos.y = (coords.y-ortho[2])/(ortho[3]-ortho[2])*2.-1.;

	gl_Position = vec4(pos, 0., 1.);

	texCoord = coords.zw;
}