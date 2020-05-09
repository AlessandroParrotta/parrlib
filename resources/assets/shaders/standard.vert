#version 130

out vec2 texCoord;

void main(void){
	gl_Position = gl_ModelViewProjectionMatrix *gl_Vertex;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	texCoord = gl_MultiTexCoord0.xy;
}