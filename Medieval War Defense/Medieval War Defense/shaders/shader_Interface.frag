#version 330 core
in vec2 TexCoord;

out vec4 FragColor; 

uniform sampler2D texture0;

void main(){ 

	vec4 TexCoordinates1; 

	TexCoordinates1 =  vec4(TexCoord, vec2(1.0f));
	
	vec4 tex1 = texture(texture0, TexCoordinates1.xy);

	FragColor = tex1; 
}