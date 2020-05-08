#version 130
uniform vec3 camPos;

in vec4 position;
in vec4 color;
in vec2 texCoordFromCPU;

 out vec4 passColor;
 out vec2 passTexCoord;

void main(void){
	gl_Position = gl_ModelViewProjectionMatrix *gl_Vertex - vec4(camPos.xyz, 0.);
	passColor = color;
	passTexCoord = texCoordFromCPU;
}