#version 130
uniform sampler2D tex;

uniform vec2 imageSize=vec2(0.);
uniform vec2 imageClosestPow=vec2(0.);

vec4 color;

in vec4 passColor;
in vec2 passTexCoord;

in vec2 texCoord;

 out vec4 outColor;

void main(void){
	texCoord = passTexCoord;
	fragCoord = gl_FragCoord.xy;
	color = passColor;
	texCoord = gl_TexCoord[0].xy;
	
	vec2 uv = fragCoord.xy/iResolution.xy;
	
	vec2 perc = imageSize/imageClosestPow;
	
	vec2 finalTexCoord = vec2(mod(passTexCoord.x*perc.x,perc.x), mod(passTexCoord.y*perc.y,perc.y));
	
	//vec2 pixVal = vec2(1./imageSize.x, 1./imageSize.y);
	
	//finalTexCoord.x = int(finalTexCoord.x*imageSize.x);
	//finalTexCoord.y = int(finalTexCoord.y*imageSize.y);
	
	//finalTexCoord.x /= imageSize.x;
	//finalTexCoord.y /= imageSize.y;
	
	outColor = texture2D(tex, finalTexCoord)*passColor;
}